#include <doctest/doctest.h>
#include <filesystem>
#include "project/Project.hpp"

TEST_CASE("project can be created and loaded") {
    const auto directory = std::filesystem::temp_directory_path() / "waypoint-project-test";
    std::filesystem::remove_all(directory);
    auto project = waypoint::project::Project::CreateDefault(directory, "TestGame");
    CHECK(project.name == "TestGame");
    auto loaded = waypoint::project::Project::Load(directory / "waypoint.project.json");
    CHECK(loaded.name == "TestGame");
    CHECK(loaded.startScene == "scenes/main.scene.json");
    std::filesystem::remove_all(directory);
}
