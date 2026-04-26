#include "MainWindow.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), graphWidget(new GraphWidget(this)) {
    setCentralWidget(graphWidget);
    setupMenuBar();
    statusBar()->showMessage("Готов к работе");
    setWindowTitle("Graph Visualizer - Визуализатор графов");
}

MainWindow::~MainWindow() {
    delete graphWidget;
}

void MainWindow::setupMenuBar() {
    QMenu* fileMenu = menuBar()->addMenu("Файл");

    QAction* exitAction = fileMenu->addAction("Выход");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu* helpMenu = menuBar()->addMenu("Справка");
    QAction* aboutAction = helpMenu->addAction("О программе");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "О программе",
                           "Graph Visualizer v1.0\n\n"
                           "Визуализатор графов на Qt\n"
                           "Особенности:\n"
                           "- Добавление/удаление вершин и рёбер\n"
                           "- Распутывание графа (удаление пересечений)\n"
                           "- Визуализация BFS и DFS\n"
                           "- Генерация случайных графов");
    });
}