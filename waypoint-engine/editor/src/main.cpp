#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char** argv) {
    QApplication application(argc, argv);
    QApplication::setApplicationName("Waypoint Editor");
    QApplication::setOrganizationName("Waypoint");
    waypoint::editor::MainWindow window;
    window.resize(1280, 800);
    window.show();
    return QApplication::exec();
}
