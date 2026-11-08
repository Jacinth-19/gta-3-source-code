#pragma once
#include <QMainWindow>
#include <filesystem>
#include "project/Project.hpp"
#include "scene/Scene.hpp"

class QTreeWidget;
class QPlainTextEdit;

namespace waypoint::editor {
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

private:
    void NewProject();
    void OpenProject();
    void SaveScene();
    void RefreshSceneTree();
    void Log(const QString& message);

    std::filesystem::path projectDirectory_;
    project::Project project_;
    scene::Scene scene_;
    QTreeWidget* sceneTree_ = nullptr;
    QPlainTextEdit* console_ = nullptr;
};
} // namespace waypoint::editor
