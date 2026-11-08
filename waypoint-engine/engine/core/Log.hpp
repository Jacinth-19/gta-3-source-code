#pragma once
#include <string_view>

namespace waypoint::log {
void Init();
void Info(std::string_view message);
void Warn(std::string_view message);
void Error(std::string_view message);
} // namespace waypoint::log
