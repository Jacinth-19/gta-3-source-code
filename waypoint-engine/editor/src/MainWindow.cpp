#include "MainWindow.hpp"
#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QFileDialog>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QToolBar>
#include <QTreeWidget>
#include "core/FileSystem.hpp"

namespace waypoint::editor {
MainWindow::MainWindow() : scene_("Main") {
    setWindowTitle("Waypoint Editor");

    auto* fileMenu = menuBar()->addMenu("&File");
    auto* newAction = fileMenu->addAction("&New Project");
    auto* openAction = fileMenu->addAction("&Open Project");
    auto* saveAction = fileMenu->addAction("&Save Scene");
    fileMenu->addSeparator();
    auto* quitAction = fileMenu->addAction("&Quit");

    connect(newAction, &QAction::triggered, this, &MainWindow::NewProject);
    connect(openAction, &QAction::triggered, this, &MainWindow::OpenProject);
    connect(saveAction, &QAction::triggered, this, &MainWindow::SaveScene);
    connect(quitAction, &QAction::triggered, this, &QApplication::quit);

    auto* toolbar = addToolBar("Main");
    toolbar->addAction(newAction);
    toolbar->addAction(openAction);
    toolbar->addAction(saveAction);

    sceneTree_ = new QTreeWidget(this);
    sceneTree_->setHeaderLabels({"Entity", "Type"});
    setCentralWidget(sceneTree_);

    console_ = new QPlainTextEdit(this);
    console_->setReadOnly(true);
    auto* consoleDock = new QDockWidget("Console", this);
    consoleDock->setWidget(console_);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);

    statusBar()->showMessage("Ready");
    Log("Welcome to Waypoint Editor. Open or create a project to begin.");
}

void MainWindow::NewProject() {
    const QString directory = QFileDialog::getExistingDirectory(this, "Choose project folder");
    if (directory.isEmpty()) {
        return;
    }
    projectDirectory_ = std::filesystem::path(directory.toStdString());
    project_ = project::Project::CreateDefault(projectDirectory_, projectDirectory_.filename().string());
    scene_ = scene::Scene("Main");
    RefreshSceneTree();
    Log("Created project: " + QString::fromStdString(project_.name));
}

void MainWindow::OpenProject() {
    const QString file = QFileDialog::getOpenFileName(this, "Open waypoint.project.json", QString(),
                                                      "Waypoint Project (*.json)");
    if (file.isEmpty()) {
        return;
    }
    const std::filesystem::path manifestPath(file.toStdString());
    projectDirectory_ = manifestPath.parent_path();
    project_ = project::Project::Load(manifestPath);
    const auto scenePath = projectDirectory_ / project_.startScene;
    if (const auto text = fs::ReadTextFile(scenePath)) {
        scene_.FromJson(*text);
    }
    RefreshSceneTree();
    Log("Opened project: " + QString::fromStdString(project_.name));
}

void MainWindow::SaveScene() {
    if (projectDirectory_.empty()) {
        Log("No project is open.");
        return;
    }
    const auto scenePath = projectDirectory_ / project_.startScene;
    if (fs::WriteTextFile(scenePath, scene_.ToJson())) {
        Log("Saved scene: " + QString::fromStdString(scenePath.string()));
    } else {
        Log("Failed to save scene.");
    }
}

void MainWindow::RefreshSceneTree() {
    sceneTree_->clear();
    auto& registry = scene_.World().Registry();
    const auto view = registry.view<const ecs::Name>();
    for (auto entity : view) {
        const auto& name = view.get<const ecs::Name>(entity);
        auto* item = new QTreeWidgetItem(sceneTree_);
        item->setText(0, QString::fromStdString(name.value));
        item->setText(1, "Entity");
    }
}

void MainWindow::Log(const QString& message) {
    if (console_) {
        console_->appendPlainText(message);
    }
}
} // namespace waypoint::editor
