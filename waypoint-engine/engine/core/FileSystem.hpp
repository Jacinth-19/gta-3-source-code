#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace waypoint::fs {
std::optional<std::string> ReadTextFile(const std::filesystem::path& path);
bool WriteTextFile(const std::filesystem::path& path, std::string_view text);
bool Exists(const std::filesystem::path& path);
bool CreateDirectories(const std::filesystem::path& path);
} // namespace waypoint::fs
