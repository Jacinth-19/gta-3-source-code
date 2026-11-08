cmake_minimum_required(VERSION 3.24)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(WaypointEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(WAYPOINT_BUILD_RUNTIME "Build game runtime/player" ON)
option(WAYPOINT_BUILD_EDITOR "Build Qt 6 editor" OFF)
option(WAYPOINT_BUILD_TESTS "Build tests" ON)
option(WAYPOINT_ENABLE_RENDER "Enable bgfx rendering module" ON)
option(WAYPOINT_ENABLE_PHYSICS "Enable Jolt physics module" ON)
option(WAYPOINT_ENABLE_SCRIPTING "Enable Lua scripting module" ON)
option(WAYPOINT_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(WAYPOINT_BUILD_BGFX_TOOLS "Build bgfx shaderc so the runtime can render meshes" ON)
option(WAYPOINT_HEADLESS_BUILD "Build without native Linux windowing (containers/CI compile checks)" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(WaypointCompiler)
include(WaypointDependencies)

add_subdirectory(engine)

if(WAYPOINT_BUILD_RUNTIME AND WAYPOINT_ENABLE_RENDER)
    add_subdirectory(runtime)
endif()

if(WAYPOINT_BUILD_EDITOR)
    add_subdirectory(editor)
endif()

if(WAYPOINT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

message(STATUS "Waypoint Engine ${PROJECT_VERSION}")
message(STATUS "  Runtime:   ${WAYPOINT_BUILD_RUNTIME}")
message(STATUS "  Editor:    ${WAYPOINT_BUILD_EDITOR}")
message(STATUS "  Render:    ${WAYPOINT_ENABLE_RENDER} (bgfx)")
message(STATUS "  Physics:   ${WAYPOINT_ENABLE_PHYSICS} (Jolt)")
message(STATUS "  Scripting: ${WAYPOINT_ENABLE_SCRIPTING} (Lua)")
