#include "GraphWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <random>

GraphWidget::GraphWidget(QWidget* parent)
    : QWidget(parent), scene(new GraphScene(this)),
      view(new QGraphicsView(scene)), algorithmRunner(new AlgorithmRunner(this)),
      nextVertexId(1), selectedVertexForEdge(-1) {
    setupUI();
    updateVisualization();
}

GraphWidget::~GraphWidget() {
    delete algorithmRunner;
    delete scene;
    delete view;
}

void GraphWidget::setupUI() {
    setWindowTitle("Graph Visualizer");
    resize(1024, 768);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Панель управления
    QHBoxLayout* controlLayout = new QHBoxLayout();

    QPushButton* addVertexBtn = new QPushButton("Добавить вершину");
    QPushButton* removeVertexBtn = new QPushButton("Удалить вершину");
    QPushButton* addEdgeBtn = new QPushButton("Добавить ребро");
    QPushButton* removeEdgeBtn = new QPushButton("Удалить ребро");
    QPushButton* untangleBtn = new QPushButton("Распутать граф");
    QPushButton* bfsBtn = new QPushButton("BFS");
    QPushButton* dfsBtn = new QPushButton("DFS");
    QPushButton* randomGraphBtn = new QPushButton("Случайный граф");
    QPushButton* clearBtn = new QPushButton("Очистить");

    controlLayout->addWidget(addVertexBtn);
    controlLayout->addWidget(removeVertexBtn);
    controlLayout->addWidget(addEdgeBtn);
    controlLayout->addWidget(removeEdgeBtn);
    controlLayout->addWidget(untangleBtn);
    controlLayout->addWidget(bfsBtn);
    controlLayout->addWidget(dfsBtn);
    controlLayout->addWidget(randomGraphBtn);
    controlLayout->addWidget(clearBtn);

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(view);

    // Подключение сигналов
    connect(addVertexBtn, &QPushButton::clicked, this, [this]() {
        int id = nextVertexId++;
        addVertexManually(id);
    });

    connect(removeVertexBtn, &QPushButton::clicked, this, [this]() {
        int id = QInputDialog::getInt(this, "Удалить вершину", "ID вершины:");
        if (id > 0) removeVertexManually(id);
    });

    connect(addEdgeBtn, &QPushButton::clicked, this, [this]() {
        int from = QInputDialog::getInt(this, "Добавить ребро", "От вершины:");
        int to = QInputDialog::getInt(this, "Добавить ребро", "До вершины:");
        if (from > 0 && to > 0) addEdgeManually(from, to);
    });

    connect(removeEdgeBtn, &QPushButton::clicked, this, [this]() {
        int from = QInputDialog::getInt(this, "Удалить ребро", "От вершины:");
        int to = QInputDialog::getInt(this, "Удалить ребро", "До вершины:");
        if (from > 0 && to > 0) removeEdgeManually(from, to);
    });

    connect(untangleBtn, &QPushButton::clicked, this, &GraphWidget::untangleGraph);
    connect(bfsBtn, &QPushButton::clicked, this, [this]() {
        int start = QInputDialog::getInt(this, "BFS", "Начать с вершины:");
        if (start > 0) startBFS(start);
    });
    connect(dfsBtn, &QPushButton::clicked, this, [this]() {
        int start = QInputDialog::getInt(this, "DFS", "Начать с вершины:");
        if (start > 0) startDFS(start);
    });
    connect(randomGraphBtn, &QPushButton::clicked, this, [this]() {
        int vertices = QInputDialog::getInt(this, "Случайный граф", "Количество вершин:", 5, 1, 20);
        int edges = QInputDialog::getInt(this, "Случайный граф", "Количество рёбер:", 7, 0, vertices*(vertices-1)/2);
        generateRandomGraph(vertices, edges);
    });
    connect(clearBtn, &QPushButton::clicked, this, &GraphWidget::clearAll);

    connect(algorithmRunner, &AlgorithmRunner::finished, this, &GraphWidget::onAlgorithmFinished);
}

void GraphWidget::addVertexManually(int id) {
    if (!graph.hasVertex(id)) {
        graph.addVertex(id);
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
    if (graph.hasVertex(startVertex)) {
        algorithmRunner->runBFS(startVertex,
            [this](int vertex, const std::vector<int>& order) {
                onAlgorithmStep(vertex, order);
            });
    }
}

void GraphWidget::startDFS(int startVertex) {
    if (graph.hasVertex(startVertex)) {
        algorithmRunner->runDFS(startVertex,
            [this](int vertex, const std::vector<int>& order) {
                onAlgorithmStep(vertex, order);
            });
    }
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
    while (addedEdges < edgeCount) {
        int from = dis(gen);
        int to = dis(gen);
        if (from != to && !graph.hasEdge(from, to)) {
            graph.addEdge(from, to);
            addedEdges++;
        }
    }

    untangleGraph();
}

void GraphWidget::clearAll() {
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
    updateVisualization();
}

void GraphWidget::updateVisualization(int highlightedVertex,
                                      const std::unordered_set<int>& visitedSet) {
    auto positions = layout.getVertexPositions();
    if (positions.empty() && graph.vertexCount() > 0) {
        // Если позиции не заданы, применяем раскладку по умолчанию
        layout.untangle(graph);
        positions = layout.getVertexPositions();
    }
    scene->updateGraph(graph, positions, highlightedVertex, visitedSet);
}