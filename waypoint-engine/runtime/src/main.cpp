#include <filesystem>
#include <iostream>
#include "app/Application.hpp"

int main(int argc, char** argv) {
    std::filesystem::path projectDirectory = std::filesystem::current_path();
    if (argc > 1) {
        projectDirectory = std::filesystem::path(argv[1]);
    }
    waypoint::app::Application application(projectDirectory);
    return application.Run();
}
