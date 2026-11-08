add_executable(waypoint_runtime src/main.cpp)
target_link_libraries(waypoint_runtime PRIVATE Waypoint::Engine)
target_compile_definitions(waypoint_runtime PRIVATE SDL_MAIN_HANDLED)
waypoint_set_target_warnings(waypoint_runtime)
set_target_properties(waypoint_runtime PROPERTIES OUTPUT_NAME waypoint)

if(WAYPOINT_ENABLE_RENDER)
    include(WaypointShaders)
    waypoint_compile_basic_shaders(waypoint_shaders "${CMAKE_CURRENT_BINARY_DIR}/shaders")
    if(TARGET waypoint_shaders)
        add_dependencies(waypoint_runtime waypoint_shaders)
        add_custom_command(TARGET waypoint_runtime POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${CMAKE_CURRENT_BINARY_DIR}/shaders"
                    "$<TARGET_FILE_DIR:waypoint_runtime>/shaders"
            COMMENT "Copying Waypoint shaders next to the runtime"
        )
    endif()
endif()
