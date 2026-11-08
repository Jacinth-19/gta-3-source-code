#pragma once
#include <filesystem>
#include <string>

namespace waypoint::project {
struct Project {
    std::string name = "MyGame";
    std::string engineVersion = "0.1.0";
    std::string startScene = "scenes/main.scene.json";
    std::string scriptEntry = "scripts/main.lua";

    static Project Load(const std::filesystem::path& manifestPath);
    static Project CreateDefault(const std::filesystem::path& directory, const std::string& name);
    bool Save(const std::filesystem::path& manifestPath) const;
    std::string ToJson() const;
};
} // namespace waypoint::project
