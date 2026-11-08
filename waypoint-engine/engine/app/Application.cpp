#include "app/Application.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/FileSystem.hpp"
#include "core/Log.hpp"
#include "scene/Systems.hpp"

namespace waypoint::app {
namespace {
constexpr float kFixedPhysicsStep = 1.0f / 60.0f;
constexpr int kMaxPhysicsStepsPerFrame = 4;

#if defined(WAYPOINT_HAS_RENDER)
std::filesystem::path RuntimeShaderDirectory() {
    const char* base = SDL_GetBasePath();
    if (base) {
        return std::filesystem::path(base) / "shaders";
    }
    return std::filesystem::current_path() / "shaders";
}

void EnsureDefaultScene(scene::Scene& scene) {
    auto& registry = scene.World().Registry();
    bool hasCamera = false;
    for (auto entity : registry.view<ecs::Camera>()) {
        (void)entity;
        hasCamera = true;
        break;
    }
    if (!hasCamera) {
        auto camera = scene.CreateEntity("Camera");
        camera.Get<ecs::Transform>().position = Vec3{0.0f, 2.0f, 8.0f};
        camera.Add<ecs::Camera>();
        camera.Add<ecs::FlyCamera>();
    }
    bool hasMesh = false;
    for (auto entity : registry.view<ecs::MeshRenderer>()) {
        (void)entity;
        hasMesh = true;
        break;
    }
    if (!hasMesh) {
        auto cube = scene.CreateEntity("Cube");
        cube.Get<ecs::Transform>().position = Vec3{0.0f, 1.0f, 0.0f};
        auto& mesh = cube.Add<ecs::MeshRenderer>();
        mesh.mesh = "cube";
        mesh.material = "default";
    }
}
#endif
} // namespace

Application::Application(std::filesystem::path projectDirectory)
    : projectDirectory_(std::move(projectDirectory)), scene_("Main") {
    project_ = project::Project::Load(projectDirectory_ / "waypoint.project.json");
    const auto scenePath = projectDirectory_ / project_.startScene;
    if (const auto text = fs::ReadTextFile(scenePath)) {
        scene_.FromJson(*text);
    }
}

int Application::Run() {
    log::Init();
    log::Info("Starting Waypoint Engine runtime");

#if defined(WAYPOINT_HAS_RENDER)
    EnsureDefaultScene(scene_);
    if (!window_.Create("Waypoint Engine", 1280, 720)) {
        return 1;
    }
    int width = 1280;
    int height = 720;
    window_.GetDrawableSize(width, height);
    if (!renderer_.Init(width, height, window_.GetNativeHandles(), RuntimeShaderDirectory())) {
        return 1;
    }
#endif

#if defined(WAYPOINT_HAS_PHYSICS)
    physics_.Init();
    {
        auto& registry = scene_.World().Registry();
        const auto view = registry.view<ecs::Transform, ecs::RigidBody>();
        for (auto entity : view) {
            auto& transform = view.get<ecs::Transform>(entity);
            auto& body = view.get<ecs::RigidBody>(entity);
            body.bodyId = physics_.AddBox(transform.position, body.halfExtent, body.dynamic);
        }
        bool hasBody = false;
        for (auto entity : registry.view<ecs::RigidBody>()) {
            (void)entity;
            hasBody = true;
            break;
        }
        if (!hasBody) {
            physics_.AddBox(Vec3{0.0f, -1.0f, 0.0f}, Vec3{50.0f, 0.5f, 50.0f}, false);
        }
    }
#endif

#if defined(WAYPOINT_HAS_SCRIPTING)
#if defined(WAYPOINT_HAS_RENDER)
    script_.BindInput(&window_.Input());
#endif
    const auto scriptPath = projectDirectory_ / project_.scriptEntry;
    if (fs::Exists(scriptPath)) {
        if (script_.LoadFile(scriptPath)) {
            script_.CallStart();
        } else {
            log::Error(script_.LastError());
        }
    }
#endif

    u64 previous = SDL_GetPerformanceCounter();
    const u64 frequency = SDL_GetPerformanceFrequency();
    float physicsAccumulator = 0.0f;
    bool running = true;
    while (running) {
#if defined(WAYPOINT_HAS_RENDER)
        window_.PollEvents();
        running = !window_.ShouldClose();
        int width = 0;
        int height = 0;
        window_.GetDrawableSize(width, height);
        renderer_.Resize(width, height);
#else
        running = false;
#endif
        const u64 now = SDL_GetPerformanceCounter();
        const float deltaTime = std::min(
            0.25f, static_cast<float>(static_cast<double>(now - previous) / frequency));
        previous = now;

#if defined(WAYPOINT_HAS_RENDER)
        scene::UpdateFlyCameras(scene_.World(), window_.Input(), deltaTime);
#endif

#if defined(WAYPOINT_HAS_PHYSICS)
        physicsAccumulator += deltaTime;
        int steps = 0;
        while (physicsAccumulator >= kFixedPhysicsStep && steps < kMaxPhysicsStepsPerFrame) {
            physics_.Step(kFixedPhysicsStep);
            physicsAccumulator -= kFixedPhysicsStep;
            ++steps;
        }
        if (steps == kMaxPhysicsStepsPerFrame) {
            physicsAccumulator = 0.0f;
        }
        {
            auto& registry = scene_.World().Registry();
            const auto view = registry.view<ecs::Transform, ecs::RigidBody>();
            for (auto entity : view) {
                auto& transform = view.get<ecs::Transform>(entity);
                const auto& body = view.get<ecs::RigidBody>(entity);
                if (body.dynamic && body.bodyId != 0) {
                    transform.position = physics_.GetPosition(body.bodyId);
                }
            }
        }
#endif

#if defined(WAYPOINT_HAS_SCRIPTING)
        script_.CallUpdate(deltaTime);
#endif

#if defined(WAYPOINT_HAS_RENDER)
        Mat4 view{1.0f};
        Mat4 projection{1.0f};
        {
            auto& registry = scene_.World().Registry();
            const auto cameras = registry.view<const ecs::Transform, const ecs::Camera>();
            for (auto entity : cameras) {
                const auto& camera = cameras.get<const ecs::Camera>(entity);
                if (!camera.primary) {
                    continue;
                }
                const auto& transform = cameras.get<const ecs::Transform>(entity);
                const Vec3 forward = transform.rotation * Vec3{0.0f, 0.0f, -1.0f};
                const Vec3 up = transform.rotation * Vec3{0.0f, 1.0f, 0.0f};
                view = glm::lookAtRH(transform.position, transform.position + forward, up);
                const float aspect = height > 0 ? static_cast<float>(width) / static_cast<float>(height)
                                                : 16.0f / 9.0f;
                if (bgfx::getCaps()->homogeneousDepth) {
                    projection = glm::perspectiveRH_NO(glm::radians(camera.verticalFovDegrees), aspect,
                                                       camera.nearPlane, camera.farPlane);
                } else {
                    projection = glm::perspectiveRH_ZO(glm::radians(camera.verticalFovDegrees), aspect,
                                                       camera.nearPlane, camera.farPlane);
                }
                break;
            }
        }
        renderer_.SetCamera(view, projection);
        renderer_.BeginFrame();
        renderer_.DrawGrid();
        {
            auto& registry = scene_.World().Registry();
            const auto meshes = registry.view<const ecs::Transform, const ecs::MeshRenderer>();
            for (auto entity : meshes) {
                const auto& mesh = meshes.get<const ecs::MeshRenderer>(entity);
                if (mesh.mesh == "cube") {
                    const auto& transform = meshes.get<const ecs::Transform>(entity);
                    renderer_.DrawCube(transform.Matrix());
                }
            }
        }
        renderer_.EndFrame();
#endif
    }

#if defined(WAYPOINT_HAS_RENDER)
    renderer_.Shutdown();
#endif
#if defined(WAYPOINT_HAS_PHYSICS)
    physics_.Shutdown();
#endif
    return 0;
}
} // namespace waypoint::app
