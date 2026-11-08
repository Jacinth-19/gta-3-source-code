#include <doctest/doctest.h>
#include <iterator>

#include "ecs/Components.hpp"
#include "platform/Input.hpp"
#include "scene/Scene.hpp"
#include "scene/Systems.hpp"

TEST_CASE("fly camera moves forward with W") {
    waypoint::scene::Scene scene;
    auto entity = scene.CreateEntity("Camera");
    entity.Add<waypoint::ecs::Camera>();
    entity.Add<waypoint::ecs::FlyCamera>();
    auto& transform = entity.Get<waypoint::ecs::Transform>();
    transform.position = waypoint::Vec3{0.0f};

    waypoint::platform::InputState input;
    input.SetKey(waypoint::platform::Key::W, true);
    waypoint::scene::UpdateFlyCameras(scene.World(), input, 1.0f);

    CHECK(transform.position.z < 0.0f);
    CHECK(transform.position.x == doctest::Approx(0.0f));
}

TEST_CASE("scene round-trips camera and mesh components") {
    waypoint::scene::Scene scene("Components");
    auto entity = scene.CreateEntity("Camera");
    entity.Add<waypoint::ecs::Camera>();
    auto& fly = entity.Add<waypoint::ecs::FlyCamera>();
    fly.yawDegrees = 42.0f;
    auto cube = scene.CreateEntity("Cube");
    auto& mesh = cube.Add<waypoint::ecs::MeshRenderer>();
    mesh.mesh = "cube";
    auto& body = cube.Add<waypoint::ecs::RigidBody>();
    body.halfExtent = waypoint::Vec3{1.0f, 2.0f, 3.0f};

    waypoint::scene::Scene loaded;
    REQUIRE(loaded.FromJson(scene.ToJson()));
    auto& registry = loaded.World().Registry();
    const auto cameras = registry.view<waypoint::ecs::Camera, waypoint::ecs::FlyCamera>();
    REQUIRE(std::distance(cameras.begin(), cameras.end()) == 1);
    for (auto e : cameras) {
        CHECK(cameras.get<waypoint::ecs::FlyCamera>(e).yawDegrees == doctest::Approx(42.0f));
    }
    const auto meshes = registry.view<waypoint::ecs::MeshRenderer, waypoint::ecs::RigidBody>();
    REQUIRE(std::distance(meshes.begin(), meshes.end()) == 1);
    for (auto e : meshes) {
        CHECK(meshes.get<waypoint::ecs::MeshRenderer>(e).mesh == "cube");
        CHECK(meshes.get<waypoint::ecs::RigidBody>(e).halfExtent.y == doctest::Approx(2.0f));
    }
}
