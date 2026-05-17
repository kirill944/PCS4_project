#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), graphWidget(new GraphWidget(this)) {
    setCentralWidget(graphWidget);
    setupMenuBar();
    setupToolBar();
    statusBar()->showMessage("Готов к работе");
    setWindowTitle("Graph Visualizer - Визуализатор графов");
    setMinimumSize(1024, 768);
    resize(1200, 800);
}

MainWindow::~MainWindow() {
}

void MainWindow::setupMenuBar() {
    // Меню Файл
    QMenu* fileMenu = menuBar()->addMenu("Файл");

    QAction* newAction = fileMenu->addAction("Новый граф");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, graphWidget, &GraphWidget::clearAll);

    QAction* saveAction = fileMenu->addAction("Сохранить");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, [this]() {
        QString filename = QFileDialog::getSaveFileName(this, "Сохранить граф",
                                                         "", "JSON файлы (*.json)");
        if (!filename.isEmpty()) {
            graphWidget->saveGraphToFile(filename);
            statusBar()->showMessage("Граф сохранён");
        }
    });

    QAction* loadAction = fileMenu->addAction("Загрузить");
    loadAction->setShortcut(QKeySequence::Open);
    connect(loadAction, &QAction::triggered, this, [this]() {
        QString filename = QFileDialog::getOpenFileName(this, "Загрузить граф",
                                                         "", "JSON файлы (*.json)");
        if (!filename.isEmpty()) {
            graphWidget->loadGraphFromFile(filename);
            statusBar()->showMessage("Граф загружен");
        }
    });

    fileMenu->addSeparator();

    QAction* exitAction = fileMenu->addAction("Выход");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Меню Вид
    QMenu* viewMenu = menuBar()->addMenu("Вид");

    QAction* zoomInAction = viewMenu->addAction("Увеличить");
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    connect(zoomInAction, &QAction::triggered, this, [this]() {
        graphWidget->view->scale(1.2, 1.2);
    });

    QAction* zoomOutAction = viewMenu->addAction("Уменьшить");
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    connect(zoomOutAction, &QAction::triggered, this, [this]() {
        graphWidget->view->scale(0.8, 0.8);
    });

    QAction* resetZoomAction = viewMenu->addAction("Сброс масштаба");
    resetZoomAction->setShortcut(QKeySequence("Ctrl+0"));
    connect(resetZoomAction, &QAction::triggered, this, [this]() {
        graphWidget->view->resetTransform();
    });

    // Меню Алгоритмы
    QMenu* algoMenu = menuBar()->addMenu("Алгоритмы");

    QAction* bfsAction = algoMenu->addAction("Поиск в ширину (BFS)");
    bfsAction->setShortcut(QKeySequence("Ctrl+B"));
    connect(bfsAction, &QAction::triggered, this, [this]() {
        bool ok;
        int start = QInputDialog::getInt(this, "BFS", "Начать с вершины:", 1, 1, 100, 1, &ok);
        if (ok) graphWidget->startBFS(start);
    });

    QAction* dfsAction = algoMenu->addAction("Поиск в глубину (DFS)");
    dfsAction->setShortcut(QKeySequence("Ctrl+D"));
    connect(dfsAction, &QAction::triggered, this, [this]() {
        bool ok;
        int start = QInputDialog::getInt(this, "DFS", "Начать с вершины:", 1, 1, 100, 1, &ok);
        if (ok) graphWidget->startDFS(start);
    });

    // Меню Справка
    QMenu* helpMenu = menuBar()->addMenu("Справка");

    QAction* aboutAction = helpMenu->addAction("О программе");
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "О программе",
                           "<h2>Graph Visualizer v1.0</h2>"
                           "<p>Визуализатор графов на Qt6</p>"
                           "<p><b>Возможности:</b></p>"
                           "<ul>"
                           "<li>Добавление/удаление вершин и рёбер</li>"
                           "<li>Автоматическое распутывание графа</li>"
                           "<li>Визуализация BFS и DFS с подсветкой</li>"
                           "<li>Генерация случайных графов</li>"
                           "<li>Сохранение/загрузка графов в JSON</li>"
                           "<li>Перетаскивание вершин мышью</li>"
                           "</ul>"
                           "<p><b>Разработчик:</b> Соловьев К.Д., ЭФБО-04-24</p>"
                           "<p><b>Год:</b> 2025</p>");
    });

    QAction* shortcutsAction = helpMenu->addAction("Горячие клавиши");
    connect(shortcutsAction, &QAction::triggered, this, [this]() {
        QMessageBox::information(this, "Горячие клавиши",
                                 "</td>"
                                 "<tr><td>Ctrl+N</b></td><td>Новый граф</td></tr>"
                                 "<tr><td><b>Ctrl+O</b></td><td>Загрузить граф</td></tr>"
                                 "<tr><td><b>Ctrl+S</b></td><td>Сохранить граф</td></tr>"
                                 "<tr><td><b>Ctrl+B</b></td><td>Запустить BFS</td></tr>"
                                 "<tr><td><b>Ctrl+D</b></td><td>Запустить DFS</td></tr>"
                                 "<tr><td><b>Ctrl++</b></td><td>Увеличить</td></tr>"
                                 "<tr><td><b>Ctrl+-</b></td><td>Уменьшить</td></tr>"
                                 "<tr><td><b>Ctrl+0</b></td><td>Сброс масштаба</td></tr>"
                                 "</table>");
    });
}

void MainWindow::setupToolBar() {
    QToolBar* toolBar = addToolBar("Инструменты");
    toolBar->setMovable(false);

    QAction* addVertexAction = toolBar->addAction("Вершина");
    connect(addVertexAction, &QAction::triggered, this, [this]() {
        static int id = 1;
        graphWidget->addVertexManually(id++);
    });

    QAction* addEdgeAction = toolBar->addAction("Ребро");
    connect(addEdgeAction, &QAction::triggered, this, [this]() {
        bool ok;
        int from = QInputDialog::getInt(this, "Ребро", "От вершины:", 1, 1, 100, 1, &ok);
        if (!ok) return;
        int to = QInputDialog::getInt(this, "Ребро", "До вершины:", 1, 1, 100, 1, &ok);
        if (!ok) return;
        graphWidget->addEdgeManually(from, to);
    });

    toolBar->addSeparator();

    QAction* untangleAction = toolBar->addAction("Распутать");
    connect(untangleAction, &QAction::triggered, graphWidget, &GraphWidget::untangleGraph);
}