#pragma once
#include <filesystem>
#include "project/Project.hpp"
#include "scene/Scene.hpp"

#if defined(WAYPOINT_HAS_RENDER)
#include "platform/SdlWindow.hpp"
#include "render/BgfxRenderer.hpp"
#endif
#if defined(WAYPOINT_HAS_PHYSICS)
#include "physics/JoltPhysicsWorld.hpp"
#endif
#if defined(WAYPOINT_HAS_SCRIPTING)
#include "script/LuaScriptEngine.hpp"
#endif

namespace waypoint::app {
class Application {
public:
    explicit Application(std::filesystem::path projectDirectory);
    int Run();

private:
    std::filesystem::path projectDirectory_;
    project::Project project_;
    scene::Scene scene_;
#if defined(WAYPOINT_HAS_RENDER)
    platform::SdlWindow window_;
    render::BgfxRenderer renderer_;
#endif
#if defined(WAYPOINT_HAS_PHYSICS)
    physics::JoltPhysicsWorld physics_;
#endif
#if defined(WAYPOINT_HAS_SCRIPTING)
    script::LuaScriptEngine script_;
#endif
};
} // namespace waypoint::app
