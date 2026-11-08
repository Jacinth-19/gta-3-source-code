find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
qt_standard_project_setup()

add_executable(waypoint_editor WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.hpp
)
target_link_libraries(waypoint_editor PRIVATE Qt6::Widgets Waypoint::Engine)
waypoint_set_target_warnings(waypoint_editor)
set_target_properties(waypoint_editor PROPERTIES OUTPUT_NAME waypoint-editor)
