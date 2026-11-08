#include "scene/Systems.hpp"

#include <algorithm>
#include <cmath>

#include <glm/gtc/quaternion.hpp>

#include "ecs/Components.hpp"

namespace waypoint::scene {
void UpdateFlyCameras(ecs::World& world, const platform::InputState& input, float deltaTime) {
    auto& registry = world.Registry();
    const auto view = registry.view<ecs::Transform, ecs::Camera, ecs::FlyCamera>();
    for (auto entity : view) {
        auto& transform = view.get<ecs::Transform>(entity);
        const auto& camera = view.get<ecs::Camera>(entity);
        auto& fly = view.get<ecs::FlyCamera>(entity);
        if (!camera.primary) {
            continue;
        }

        const bool lookActive = input.relativeMouse || input.IsMouseDown(platform::MouseButton::Right);
        if (lookActive) {
            fly.yawDegrees -= input.mouseDelta.x * fly.lookSensitivity;
            fly.pitchDegrees -= input.mouseDelta.y * fly.lookSensitivity;
            fly.pitchDegrees = std::clamp(fly.pitchDegrees, -89.0f, 89.0f);
        }

        const Quat yaw = glm::angleAxis(glm::radians(fly.yawDegrees), Vec3{0.0f, 1.0f, 0.0f});
        const Quat pitch = glm::angleAxis(glm::radians(fly.pitchDegrees), Vec3{1.0f, 0.0f, 0.0f});
        transform.rotation = glm::normalize(yaw * pitch);

        const Vec3 forward = transform.rotation * Vec3{0.0f, 0.0f, -1.0f};
        const Vec3 right = transform.rotation * Vec3{1.0f, 0.0f, 0.0f};
        Vec3 move{0.0f};
        if (input.IsKeyDown(platform::Key::W)) move += forward;
        if (input.IsKeyDown(platform::Key::S)) move -= forward;
        if (input.IsKeyDown(platform::Key::D)) move += right;
        if (input.IsKeyDown(platform::Key::A)) move -= right;
        if (input.IsKeyDown(platform::Key::Space)) move += Vec3{0.0f, 1.0f, 0.0f};
        if (input.IsKeyDown(platform::Key::LeftControl) || input.IsKeyDown(platform::Key::RightControl)) {
            move -= Vec3{0.0f, 1.0f, 0.0f};
        }

        const f32 lengthSquared = glm::dot(move, move);
        if (lengthSquared > 0.0001f) {
            move /= std::sqrt(lengthSquared);
            f32 speed = fly.moveSpeed;
            if (input.IsKeyDown(platform::Key::LeftShift) || input.IsKeyDown(platform::Key::RightShift)) {
                speed *= fly.boostMultiplier;
            }
            transform.position += move * speed * deltaTime;
        }
    }
}
} // namespace waypoint::scene
