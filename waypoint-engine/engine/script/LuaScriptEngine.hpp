#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

#include "platform/Input.hpp"

namespace waypoint::script {
class LuaScriptEngine {
public:
    LuaScriptEngine();
    ~LuaScriptEngine();

    // The input state must outlive the script engine. Safe to call with nullptr.
    void BindInput(const platform::InputState* input);
    bool LoadFile(const std::filesystem::path& path);
    bool LoadString(std::string_view code);
    void CallStart();
    void CallUpdate(float deltaTime);
    std::optional<double> GetNumber(std::string_view name) const;
    std::string LastError() const;

private:
    struct Impl;
    Impl* impl_;
};
} // namespace waypoint::script
