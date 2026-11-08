#include <doctest/doctest.h>
#include <filesystem>
#include "core/FileSystem.hpp"
#include "core/Uuid.hpp"

TEST_CASE("uuid looks like a uuid") {
    const auto uuid = waypoint::GenerateUuid();
    CHECK(uuid.size() == 36);
    CHECK(uuid[8] == '-');
    CHECK(uuid[13] == '-');
    CHECK(uuid[18] == '-');
    CHECK(uuid[23] == '-');
}

TEST_CASE("filesystem can write and read text") {
    const auto path = std::filesystem::temp_directory_path() / "waypoint-test.txt";
    REQUIRE(waypoint::fs::WriteTextFile(path, "hello"));
    const auto text = waypoint::fs::ReadTextFile(path);
    REQUIRE(text.has_value());
    CHECK(*text == "hello");
    std::filesystem::remove(path);
}
