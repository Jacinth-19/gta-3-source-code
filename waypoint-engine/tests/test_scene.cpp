#include <doctest/doctest.h>
#include <iterator>
#include "scene/Scene.hpp"

TEST_CASE("scene can create entities and round-trip json") {
    waypoint::scene::Scene scene("Test");
    auto entity = scene.CreateEntity("Player");
    entity.Get<waypoint::ecs::Transform>().position = waypoint::Vec3{1.0f, 2.0f, 3.0f};

    const auto json = scene.ToJson();
    waypoint::scene::Scene loaded;
    REQUIRE(loaded.FromJson(json));
    CHECK(loaded.Name() == "Test");

    auto& registry = loaded.World().Registry();
    const auto view = registry.view<const waypoint::ecs::Name, const waypoint::ecs::Transform>();
    REQUIRE(std::distance(view.begin(), view.end()) == 1);
    for (auto e : view) {
        CHECK(view.get<const waypoint::ecs::Name>(e).value == "Player");
        CHECK(view.get<const waypoint::ecs::Transform>(e).position.x == doctest::Approx(1.0f));
    }
}
