add_library(waypoint_engine STATIC)
add_library(Waypoint::Engine ALIAS waypoint_engine)

target_include_directories(waypoint_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(waypoint_engine PUBLIC cxx_std_20)

target_sources(waypoint_engine PRIVATE
    core/Log.cpp
    core/FileSystem.cpp
    core/Uuid.cpp
    ecs/World.cpp
    scene/Scene.cpp
    scene/Systems.cpp
    project/Project.cpp
    platform/Input.cpp
)

target_link_libraries(waypoint_engine PUBLIC
    EnTT::EnTT
    glm::glm
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

if(WAYPOINT_ENABLE_SCRIPTING)
    target_sources(waypoint_engine PRIVATE script/LuaScriptEngine.cpp)
    target_link_libraries(waypoint_engine PUBLIC sol2::sol2 waypoint_lua)
    target_compile_definitions(waypoint_engine PUBLIC WAYPOINT_HAS_SCRIPTING=1)
endif()

if(WAYPOINT_ENABLE_PHYSICS)
    target_sources(waypoint_engine PRIVATE physics/JoltPhysicsWorld.cpp)
    target_link_libraries(waypoint_engine PUBLIC Jolt)
    target_compile_definitions(waypoint_engine PUBLIC WAYPOINT_HAS_PHYSICS=1)
endif()

if(WAYPOINT_ENABLE_RENDER)
    target_sources(waypoint_engine PRIVATE
        platform/SdlWindow.cpp
        render/BgfxRenderer.cpp
    )
    target_link_libraries(waypoint_engine PUBLIC ${WAYPOINT_SDL_TARGET} ${WAYPOINT_BGFX_TARGET})
    target_compile_definitions(waypoint_engine PUBLIC SDL_MAIN_HANDLED)
    target_compile_definitions(waypoint_engine PUBLIC WAYPOINT_HAS_RENDER=1)
endif()

if(WAYPOINT_BUILD_RUNTIME AND WAYPOINT_ENABLE_RENDER)
    target_sources(waypoint_engine PRIVATE app/Application.cpp)
    target_compile_definitions(waypoint_engine PUBLIC WAYPOINT_HAS_APP=1)
endif()

waypoint_set_target_warnings(waypoint_engine)
