#include "script/LuaScriptEngine.hpp"

#include <sol/sol.hpp>
#include <tuple>

#include "core/FileSystem.hpp"
#include "core/Log.hpp"

namespace waypoint::script {
struct LuaScriptEngine::Impl {
    sol::state lua;
    std::string lastError;
    const platform::InputState* input = nullptr;
};

LuaScriptEngine::LuaScriptEngine() : impl_(new Impl) {
    impl_->lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math,
                              sol::lib::table, sol::lib::os);
    auto waypoint = impl_->lua.create_named_table("waypoint");
    waypoint.set_function("log", [](const std::string& message) { log::Info(message); });
    waypoint.set_function("warn", [](const std::string& message) { log::Warn(message); });
    waypoint.set_function("error", [](const std::string& message) { log::Error(message); });

    auto input = waypoint.create_named("input");
    input.set_function("is_key_down", [this](const std::string& name) {
        return impl_->input && impl_->input->IsKeyDown(platform::KeyFromString(name));
    });
    input.set_function("is_key_pressed", [this](const std::string& name) {
        return impl_->input && impl_->input->IsKeyPressed(platform::KeyFromString(name));
    });
    input.set_function("is_mouse_down", [this](int button) {
        if (!impl_->input || button < 0 || button >= static_cast<int>(platform::MouseButton::Count)) {
            return false;
        }
        return impl_->input->IsMouseDown(static_cast<platform::MouseButton>(button));
    });
    input.set_function("mouse_delta", [this]() {
        if (!impl_->input) {
            return std::make_tuple(0.0f, 0.0f);
        }
        return std::make_tuple(impl_->input->mouseDelta.x, impl_->input->mouseDelta.y);
    });
}

void LuaScriptEngine::BindInput(const platform::InputState* input) {
    impl_->input = input;
}

LuaScriptEngine::~LuaScriptEngine() {
    delete impl_;
}

bool LuaScriptEngine::LoadFile(const std::filesystem::path& path) {
    const auto text = fs::ReadTextFile(path);
    if (!text) {
        impl_->lastError = "Could not read script: " + path.string();
        return false;
    }
    return LoadString(*text);
}

bool LuaScriptEngine::LoadString(std::string_view code) {
    const auto result = impl_->lua.safe_script(code, sol::script_pass_on_error);
    if (!result.valid()) {
        sol::error error = result;
        impl_->lastError = error.what();
        return false;
    }
    impl_->lastError.clear();
    return true;
}

void LuaScriptEngine::CallStart() {
    sol::protected_function function = impl_->lua["on_start"];
    if (!function.valid()) {
        return;
    }
    auto result = function();
    if (!result.valid()) {
        sol::error error = result;
        impl_->lastError = error.what();
        log::Error(impl_->lastError);
    }
}

void LuaScriptEngine::CallUpdate(float deltaTime) {
    sol::protected_function function = impl_->lua["on_update"];
    if (!function.valid()) {
        return;
    }
    auto result = function(deltaTime);
    if (!result.valid()) {
        sol::error error = result;
        impl_->lastError = error.what();
        log::Error(impl_->lastError);
    }
}

std::optional<double> LuaScriptEngine::GetNumber(std::string_view name) const {
    const sol::object object = impl_->lua[std::string(name)];
    if (!object.is<double>()) {
        return std::nullopt;
    }
    return object.as<double>();
}

std::string LuaScriptEngine::LastError() const {
    return impl_->lastError;
}
} // namespace waypoint::script
