#include "GraphWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <random>
#include <QGroupBox>
#include <QCheckBox>
#include <QTimer>
#include <queue>
#include <stack>

GraphWidget::GraphWidget(QWidget* parent)
    : QWidget(parent), scene(new GraphScene(this)),
      view(new QGraphicsView(scene)), algorithmRunner(new AlgorithmRunner(this)),
      untangleTimer(new QTimer(this)), nextVertexId(1), selectedVertexForEdge(-1),
      isAlgorithmRunning(false), isStepByStepMode(false), currentStepIndex(0) {
    setupUI();
    updateVisualization();

    connect(algorithmRunner, &AlgorithmRunner::finished, this, &GraphWidget::onAlgorithmFinished);
    connect(scene, &GraphScene::vertexMoved, this, &GraphWidget::onVertexMoved);

    untangleTimer->setSingleShot(true);
    connect(untangleTimer, &QTimer::timeout, this, &GraphWidget::onAutoUntangle);
}

GraphWidget::~GraphWidget() {
    delete algorithmRunner;
}

void GraphWidget::setupUI() {
    setWindowTitle("Graph Visualizer");
    resize(1200, 800);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Верхняя панель инструментов
    QHBoxLayout* topControlLayout = new QHBoxLayout();

    QGroupBox* vertexGroup = new QGroupBox("Вершины");
    QHBoxLayout* vertexLayout = new QHBoxLayout();
    QPushButton* addVertexBtn = new QPushButton("Добавить");
    QPushButton* removeVertexBtn = new QPushButton("Удалить");
    vertexLayout->addWidget(addVertexBtn);
    vertexLayout->addWidget(removeVertexBtn);
    vertexGroup->setLayout(vertexLayout);

    QGroupBox* edgeGroup = new QGroupBox("Рёбра");
    QHBoxLayout* edgeLayout = new QHBoxLayout();
    QPushButton* addEdgeBtn = new QPushButton("Добавить");
    QPushButton* removeEdgeBtn = new QPushButton("Удалить");
    edgeLayout->addWidget(addEdgeBtn);
    edgeLayout->addWidget(removeEdgeBtn);
    edgeGroup->setLayout(edgeLayout);

    QGroupBox* algorithmGroup = new QGroupBox("Алгоритмы");
    QHBoxLayout* algorithmLayout = new QHBoxLayout();
    QPushButton* bfsBtn = new QPushButton("BFS");
    QPushButton* dfsBtn = new QPushButton("DFS");
    QPushButton* nextStepBtn = new QPushButton("Следующий шаг");
    nextStepBtn->setEnabled(false);
    algorithmLayout->addWidget(bfsBtn);
    algorithmLayout->addWidget(dfsBtn);
    algorithmLayout->addWidget(nextStepBtn);
    algorithmGroup->setLayout(algorithmLayout);

    QGroupBox* layoutGroup = new QGroupBox("Раскладка");
    QHBoxLayout* layoutLayout = new QHBoxLayout();
    QPushButton* untangleBtn = new QPushButton("Распутать");
    QCheckBox* autoUntangleCheck = new QCheckBox("Авто");
    layoutLayout->addWidget(untangleBtn);
    layoutLayout->addWidget(autoUntangleCheck);
    layoutGroup->setLayout(layoutLayout);

    topControlLayout->addWidget(vertexGroup);
    topControlLayout->addWidget(edgeGroup);
    topControlLayout->addWidget(algorithmGroup);
    topControlLayout->addWidget(layoutGroup);
    topControlLayout->addStretch();

    // Нижняя панель инструментов
    QHBoxLayout* bottomControlLayout = new QHBoxLayout();

    QPushButton* randomGraphBtn = new QPushButton("Случайный граф");
    QPushButton* saveGraphBtn = new QPushButton("Сохранить");
    QPushButton* loadGraphBtn = new QPushButton("Загрузить");
    QPushButton* clearBtn = new QPushButton("Очистить всё");
    QLabel* statusLabel = new QLabel("Готов к работе");

    bottomControlLayout->addWidget(randomGraphBtn);
    bottomControlLayout->addWidget(saveGraphBtn);
    bottomControlLayout->addWidget(loadGraphBtn);
    bottomControlLayout->addWidget(clearBtn);
    bottomControlLayout->addStretch();
    bottomControlLayout->addWidget(statusLabel);

    mainLayout->addLayout(topControlLayout);
    mainLayout->addWidget(view, 1);
    mainLayout->addLayout(bottomControlLayout);

    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);

    // Подключение сигналов
    connect(addVertexBtn, &QPushButton::clicked, this, [this, statusLabel]() {
        int id = nextVertexId++;
        addVertexManually(id);
        statusLabel->setText(QString("Добавлена вершина %1").arg(id));
    });

    connect(removeVertexBtn, &QPushButton::clicked, this, [this, statusLabel]() {
        bool ok;
        int id = QInputDialog::getInt(this, "Удалить вершину",
                                       "ID вершины:", 1, 1, 100, 1, &ok);
        if (ok && graph.hasVertex(id)) {
            removeVertexManually(id);
            statusLabel->setText(QString("Удалена вершина %1").arg(id));
        } else if (ok) {
            QMessageBox::warning(this, "Ошибка", "Вершина не найдена!");
        }
    });

    connect(addEdgeBtn, &QPushButton::clicked, this, [this, statusLabel]() {
        bool ok;
        int from = QInputDialog::getInt(this, "Добавить ребро",
                                         "От вершины:", 1, 1, 100, 1, &ok);
        if (!ok) return;
        int to = QInputDialog::getInt(this, "Добавить ребро",
                                       "До вершины:", 1, 1, 100, 1, &ok);
        if (!ok) return;

        if (graph.hasVertex(from) && graph.hasVertex(to)) {
            if (!graph.hasEdge(from, to)) {
                addEdgeManually(from, to);
                statusLabel->setText(QString("Добавлено ребро %1-%2").arg(from).arg(to));
            } else {
                QMessageBox::warning(this, "Ошибка", "Ребро уже существует!");
            }
        } else {
            QMessageBox::warning(this, "Ошибка", "Вершина не найдена!");
        }
    });

    connect(removeEdgeBtn, &QPushButton::clicked, this, [this, statusLabel]() {
        bool ok;
        int from = QInputDialog::getInt(this, "Удалить ребро",
                                         "От вершины:", 1, 1, 100, 1, &ok);
        if (!ok) return;
        int to = QInputDialog::getInt(this, "Удалить ребро",
                                       "До вершины:", 1, 1, 100, 1, &ok);
        if (!ok) return;

        if (graph.hasEdge(from, to)) {
            removeEdgeManually(from, to);
            statusLabel->setText(QString("Удалено ребро %1-%2").arg(from).arg(to));
        } else {
            QMessageBox::warning(this, "Ошибка", "Ребро не найдено!");
        }
    });

    connect(untangleBtn, &QPushButton::clicked, this, [this, statusLabel]() {
        untangleGraph();
        statusLabel->setText("Применена новая раскладка");
    });

    connect(autoUntangleCheck, &QCheckBox::toggled, this, [this](bool checked) {
        if (checked) {
            untangleTimer->start(2000);
        } else {
            untangleTimer->stop();
        }
    });

    connect(bfsBtn, &QPushButton::clicked, this, [this, nextStepBtn, statusLabel]() {
        if (graph.vertexCount() == 0) {
            QMessageBox::warning(this, "Ошибка", "Граф пуст! Добавьте вершины.");
            return;
        }
        bool ok;
        int start = QInputDialog::getInt(this, "BFS - Поиск в ширину",
                                          "Начать с вершины:", 1, 1, 100, 1, &ok);
        if (ok && graph.hasVertex(start)) {
            // Останавливаем текущий алгоритм если запущен
            if (algorithmRunner->isRunning()) {
                algorithmRunner->stop();
            }

            isStepByStepMode = true;
            isAlgorithmRunning = true;
            currentStepIndex = 0;
            scene->setVertexDraggable(false);

            // Вычисляем порядок обхода BFS
            currentTraversalOrder = algorithmRunner->getBFSTraversal(graph, start);

            nextStepBtn->setEnabled(true);
            statusLabel->setText(QString("BFS с вершины %1 (пошаговый режим)").arg(start));

            // Показываем первый шаг
            nextAlgorithmStep();
        } else if (ok) {
            QMessageBox::warning(this, "Ошибка", "Вершина не найдена!");
        }
    });

    connect(dfsBtn, &QPushButton::clicked, this, [this, nextStepBtn, statusLabel]() {
        if (graph.vertexCount() == 0) {
            QMessageBox::warning(this, "Ошибка", "Граф пуст! Добавьте вершины.");
            return;
        }
        bool ok;
        int start = QInputDialog::getInt(this, "DFS - Поиск в глубину",
                                          "Начать с вершины:", 1, 1, 100, 1, &ok);
        if (ok && graph.hasVertex(start)) {
            // Останавливаем текущий алгоритм если запущен
            if (algorithmRunner->isRunning()) {
                algorithmRunner->stop();
            }

            isStepByStepMode = true;
            isAlgorithmRunning = true;
            currentStepIndex = 0;
            scene->setVertexDraggable(false);

            // Вычисляем порядок обхода DFS
            currentTraversalOrder = algorithmRunner->getDFSTraversal(graph, start);

            nextStepBtn->setEnabled(true);
            statusLabel->setText(QString("DFS с вершины %1 (пошаговый режим)").arg(start));

            // Показываем первый шаг
            nextAlgorithmStep();
        } else if (ok) {
            QMessageBox::warning(this, "Ошибка", "Вершина не найдена!");
        }
    });

    connect(nextStepBtn, &QPushButton::clicked, this, &GraphWidget::nextAlgorithmStep);

    connect(randomGraphBtn, &QPushButton::clicked, this, [this, statusLabel]() {
        bool ok;
        int vertices = QInputDialog::getInt(this, "Случайный граф",
                                             "Количество вершин:", 5, 1, 20, 1, &ok);
        if (!ok) return;
        int maxEdges = vertices * (vertices - 1) / 2;
        int edges = QInputDialog::getInt(this, "Случайный граф",
                                          "Количество рёбер:", vertices, 0, maxEdges, 1, &ok);
        if (!ok) return;
        generateRandomGraph(vertices, edges);
        statusLabel->setText(QString("Сгенерирован случайный граф: %1 вершин, %2 рёбер")
                            .arg(vertices).arg(edges));
    });

    connect(saveGraphBtn, &QPushButton::clicked, this, [this, statusLabel]() {
        QString filename = QFileDialog::getSaveFileName(this, "Сохранить граф",
                                                         "", "JSON файлы (*.json)");
        if (!filename.isEmpty()) {
            saveGraphToFile(filename);
            statusLabel->setText(QString("Граф сохранён в %1").arg(filename));
        }
    });

    connect(loadGraphBtn, &QPushButton::clicked, this, [this, statusLabel]() {
        QString filename = QFileDialog::getOpenFileName(this, "Загрузить граф",
                                                         "", "JSON файлы (*.json)");
        if (!filename.isEmpty()) {
            loadGraphFromFile(filename);
            statusLabel->setText(QString("Граф загружен из %1").arg(filename));
        }
    });

    connect(clearBtn, &QPushButton::clicked, this, [this, statusLabel, nextStepBtn]() {
        clearAll();
        nextStepBtn->setEnabled(false);
        statusLabel->setText("Граф очищен");
    });
}

void GraphWidget::addVertexManually(int id) {
    if (!graph.hasVertex(id)) {
        graph.addVertex(id);

        // Получаем центр видимости view
        if (view && view->viewport()) {
            QPointF center = view->mapToScene(view->viewport()->rect().center());
            layout.setVertexPosition(id, center);
        } else {
            // Дефолтная позиция, если view ещё не готов
            layout.setVertexPosition(id, QPointF(400, 300));
        }

        updateVisualization();
    }
}

void GraphWidget::removeVertexManually(int id) {
    if (graph.hasVertex(id)) {
        graph.removeVertex(id);
        updateVisualization();
    }
}

void GraphWidget::addEdgeManually(int from, int to) {
    if (graph.hasVertex(from) && graph.hasVertex(to) && !graph.hasEdge(from, to)) {
        graph.addEdge(from, to);
        updateVisualization();
    }
}

void GraphWidget::removeEdgeManually(int from, int to) {
    if (graph.hasEdge(from, to)) {
        graph.removeEdge(from, to);
        updateVisualization();
    }
}

void GraphWidget::untangleGraph() {
    layout.untangle(graph);
    updateVisualization();
}

void GraphWidget::startBFS(int startVertex) {
    if (algorithmRunner->isRunning()) {
        algorithmRunner->stop();
    }
    isAlgorithmRunning = true;
    isStepByStepMode = false;
    scene->setVertexDraggable(false);
    algorithmRunner->runBFS(graph, startVertex,
        [this](int vertex, const std::vector<int>& order) {
            onAlgorithmStep(vertex, order);
        });
}

void GraphWidget::startDFS(int startVertex) {
    if (algorithmRunner->isRunning()) {
        algorithmRunner->stop();
    }
    isAlgorithmRunning = true;
    isStepByStepMode = false;
    scene->setVertexDraggable(false);
    algorithmRunner->runDFS(graph, startVertex,
        [this](int vertex, const std::vector<int>& order) {
            onAlgorithmStep(vertex, order);
        });
}

void GraphWidget::nextAlgorithmStep() {
    if (!isStepByStepMode) return;

    if (currentStepIndex < static_cast<int>(currentTraversalOrder.size())) {
        int vertex = currentTraversalOrder[currentStepIndex];
        std::vector<int> partialOrder(currentTraversalOrder.begin(),
                                       currentTraversalOrder.begin() + currentStepIndex + 1);
        onAlgorithmStep(vertex, partialOrder);
        currentStepIndex++;
    } else {
        // Алгоритм завершён
        finishAlgorithm();
    }
}

void GraphWidget::finishAlgorithm() {
    isStepByStepMode = false;
    isAlgorithmRunning = false;
    scene->setVertexDraggable(true);
    onAlgorithmFinished();
}

void GraphWidget::generateRandomGraph(int vertexCount, int edgeCount) {
    clearAll();

    // Добавляем вершины
    for (int i = 1; i <= vertexCount; ++i) {
        graph.addVertex(i);
        nextVertexId = i + 1;
    }

    // Добавляем случайные рёбра
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, vertexCount);

    int addedEdges = 0;
    int maxAttempts = edgeCount * 10;
    int attempts = 0;

    while (addedEdges < edgeCount && attempts < maxAttempts) {
        int from = dis(gen);
        int to = dis(gen);
        if (from != to && !graph.hasEdge(from, to)) {
            graph.addEdge(from, to);
            addedEdges++;
        }
        attempts++;
    }

    untangleGraph();
}

void GraphWidget::saveGraphToFile(const QString& filename) {
    QJsonObject root;

    // Сохраняем вершины
    QJsonArray verticesArray;
    auto vertices = graph.getVertices();
    for (int v : vertices) {
        verticesArray.append(v);
    }
    root["vertices"] = verticesArray;

    // Сохраняем рёбра
    QJsonArray edgesArray;
    for (int v : vertices) {
        auto neighbors = graph.getNeighbors(v);
        for (int n : neighbors) {
            if (v < n) {
                QJsonObject edge;
                edge["from"] = v;
                edge["to"] = n;
                edgesArray.append(edge);
            }
        }
    }
    root["edges"] = edgesArray;

    // Сохраняем позиции вершин
    auto positions = layout.getVertexPositions();
    QJsonObject positionsObj;
    for (const auto& pair : positions) {
        QJsonObject pos;
        pos["x"] = pair.second.x();
        pos["y"] = pair.second.y();
        positionsObj[QString::number(pair.first)] = pos;
    }
    root["positions"] = positionsObj;
    root["nextVertexId"] = nextVertexId;

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}

void GraphWidget::loadGraphFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();

    clearAll();

    // Загружаем вершины
    QJsonArray verticesArray = root["vertices"].toArray();
    for (const auto& v : verticesArray) {
        int vertex = v.toInt();
        graph.addVertex(vertex);
        if (vertex >= nextVertexId) nextVertexId = vertex + 1;
    }

    // Загружаем рёбра
    QJsonArray edgesArray = root["edges"].toArray();
    for (const auto& edge : edgesArray) {
        QJsonObject edgeObj = edge.toObject();
        int from = edgeObj["from"].toInt();
        int to = edgeObj["to"].toInt();
        graph.addEdge(from, to);
    }

    // Загружаем позиции
    QJsonObject positionsObj = root["positions"].toObject();
    for (const auto& key : positionsObj.keys()) {
        int vertex = key.toInt();
        QJsonObject posObj = positionsObj[key].toObject();
        QPointF pos(posObj["x"].toDouble(), posObj["y"].toDouble());
        layout.setVertexPosition(vertex, pos);
    }

    if (root.contains("nextVertexId")) {
        nextVertexId = root["nextVertexId"].toInt();
    }

    updateVisualization();
}

void GraphWidget::clearAll() {
    if (algorithmRunner->isRunning()) {
        algorithmRunner->stop();
    }
    isAlgorithmRunning = false;
    isStepByStepMode = false;
    currentStepIndex = 0;
    currentTraversalOrder.clear();
    scene->setVertexDraggable(true);
    graph.clear();
    nextVertexId = 1;
    layout.reset();
    updateVisualization();
}

void GraphWidget::onAlgorithmStep(int currentVertex, const std::vector<int>& order) {
    std::unordered_set<int> visited(order.begin(), order.end());
    updateVisualization(currentVertex, visited);
}

void GraphWidget::onAlgorithmFinished() {
    isAlgorithmRunning = false;
    isStepByStepMode = false;
    scene->setVertexDraggable(true);
    updateVisualization();

    // Находим кнопку "Следующий шаг" и отключаем её
    auto btns = findChildren<QPushButton*>();
    for (auto btn : btns) {
        if (btn->text() == "Следующий шаг") {
            btn->setEnabled(false);
            break;
        }
    }
}

void GraphWidget::onVertexMoved(int vertex, const QPointF& pos) {
    layout.setVertexPosition(vertex, pos);

    // Не нужно вызывать updateEdgePositions, так как это уже делается в updateVertexPosition
    // Просто обновляем позицию в layout

    if (!isAlgorithmRunning && !isStepByStepMode && untangleTimer->isActive()) {
        untangleTimer->stop();
        untangleTimer->start(2000);
    }
}

void GraphWidget::onAutoUntangle() {
    if (!isAlgorithmRunning && !isStepByStepMode) {
        untangleGraph();
    }
}

void GraphWidget::syncPositionsFromScene() {
    auto draggedPositions = scene->getDraggedPositions();
    for (const auto& pair : draggedPositions) {
        layout.setVertexPosition(pair.first, pair.second);
    }
}

void GraphWidget::updateVisualization(int highlightedVertex,
                                      const std::unordered_set<int>& visitedSet) {
    auto positions = layout.getVertexPositions();

    // Если позиции не заданы и граф не пуст, применяем раскладку
    if (positions.empty() && graph.vertexCount() > 0 && !isAlgorithmRunning && !isStepByStepMode) {
        layout.untangle(graph);
        positions = layout.getVertexPositions();
    }

    scene->updateGraph(graph, positions, highlightedVertex, visitedSet);

    // Принудительно обновляем вид
    view->viewport()->update();
}