#include "core/Log.hpp"
#include <spdlog/spdlog.h>

namespace waypoint::log {
void Init() {
    spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
}

void Info(std::string_view message) {
    spdlog::info("{}", message);
}

void Warn(std::string_view message) {
    spdlog::warn("{}", message);
}

void Error(std::string_view message) {
    spdlog::error("{}", message);
}
} // namespace waypoint::log
