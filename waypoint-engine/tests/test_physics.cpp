#include <doctest/doctest.h>

#include "physics/JoltPhysicsWorld.hpp"

TEST_CASE("jolt box falls under gravity") {
    waypoint::physics::JoltPhysicsWorld world;
    REQUIRE(world.Init());
    const auto floor = world.AddBox(waypoint::Vec3{0.0f, -1.0f, 0.0f}, waypoint::Vec3{10.0f, 0.5f, 10.0f}, false);
    const auto box = world.AddBox(waypoint::Vec3{0.0f, 5.0f, 0.0f}, waypoint::Vec3{0.5f, 0.5f, 0.5f}, true);
    REQUIRE(floor != 0);
    REQUIRE(box != 0);

    for (int i = 0; i < 120; ++i) {
        world.Step(1.0f / 60.0f);
    }

    const auto position = world.GetPosition(box);
    CHECK(position.y < 4.0f);
    CHECK(position.y > -0.5f);
    world.Shutdown();
}
