#include "project/Project.hpp"
#include <nlohmann/json.hpp>
#include "core/FileSystem.hpp"

namespace waypoint::project {
using nlohmann::json;

Project Project::Load(const std::filesystem::path& manifestPath) {
    Project project;
    const auto text = fs::ReadTextFile(manifestPath);
    if (!text) {
        return project;
    }
    try {
        const json root = json::parse(*text);
        project.name = root.value("name", project.name);
        project.engineVersion = root.value("engineVersion", project.engineVersion);
        project.startScene = root.value("startScene", project.startScene);
        project.scriptEntry = root.value("scriptEntry", project.scriptEntry);
    } catch (...) {
    }
    return project;
}

Project Project::CreateDefault(const std::filesystem::path& directory, const std::string& name) {
    Project project;
    project.name = name;
    fs::CreateDirectories(directory / "scenes");
    fs::CreateDirectories(directory / "scripts");
    fs::CreateDirectories(directory / "assets");
    project.Save(directory / "waypoint.project.json");
    fs::WriteTextFile(directory / project.startScene,
                      "{\n  \"name\": \"Main\",\n  \"entities\": []\n}\n");
    fs::WriteTextFile(directory / project.scriptEntry,
                      "-- Waypoint Engine beginner script\nfunction on_start()\n  waypoint.log(\"Hello from Lua!\")\nend\n\nfunction on_update(dt)\nend\n");
    return project;
}

bool Project::Save(const std::filesystem::path& manifestPath) const {
    return fs::WriteTextFile(manifestPath, ToJson());
}

std::string Project::ToJson() const {
    const json root{
        {"name", name},
        {"engineVersion", engineVersion},
        {"startScene", startScene},
        {"scriptEntry", scriptEntry}
    };
    return root.dump(2) + "\n";
}
} // namespace waypoint::project
