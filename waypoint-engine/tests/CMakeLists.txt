add_executable(waypoint_tests
    test_main.cpp
    test_core.cpp
    test_scene.cpp
    test_project.cpp
    test_input.cpp
    test_systems.cpp
)

if(WAYPOINT_ENABLE_SCRIPTING)
    target_sources(waypoint_tests PRIVATE test_script.cpp)
endif()

if(WAYPOINT_ENABLE_PHYSICS)
    target_sources(waypoint_tests PRIVATE test_physics.cpp)
endif()

target_link_libraries(waypoint_tests PRIVATE Waypoint::Engine doctest::doctest)
waypoint_set_target_warnings(waypoint_tests)
add_test(NAME waypoint_tests COMMAND waypoint_tests)
