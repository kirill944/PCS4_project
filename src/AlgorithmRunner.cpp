#include "AlgorithmRunner.h"

AlgorithmRunner::AlgorithmRunner(QObject* parent)
    : QObject(parent), timer(new QTimer(this)), running(false), currentStepIndex(0) {
    timer->setSingleShot(true);
}

AlgorithmRunner::~AlgorithmRunner() {
    stop();
}

void AlgorithmRunner::runBFS(const Graph& graph, int startVertex,
                             std::function<void(int, const std::vector<int>&)> stepCallback) {
    if (running) return;

    running = true;
    currentStepIndex = 0;
    traversalOrder.clear();

    std::queue<int> q;
    std::unordered_set<int> visited;
    std::vector<int> order;

    q.push(startVertex);
    visited.insert(startVertex);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        order.push_back(current);

        auto neighbors = graph.getNeighbors(current);
        for (int neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    traversalOrder = order;

    // Запускаем пошаговое выполнение
    executeNextStep(stepCallback);
}

void AlgorithmRunner::runDFS(const Graph& graph, int startVertex,
                             std::function<void(int, const std::vector<int>&)> stepCallback) {
    if (running) return;

    running = true;
    currentStepIndex = 0;
    traversalOrder.clear();

    std::stack<int> s;
    std::unordered_set<int> visited;
    std::vector<int> order;

    s.push(startVertex);

    while (!s.empty()) {
        int current = s.top();
        s.pop();

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            order.push_back(current);

            auto neighbors = graph.getNeighbors(current);
            // Добавляем соседей в обратном порядке для правильного порядка DFS
            for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
                if (visited.find(*it) == visited.end()) {
                    s.push(*it);
                }
            }
        }
    }

    traversalOrder = order;
    executeNextStep(stepCallback);
}

void AlgorithmRunner::executeNextStep(std::function<void(int, const std::vector<int>&)> callback) {
    if (!running) return;

    if (currentStepIndex < static_cast<int>(traversalOrder.size())) {
        int vertex = traversalOrder[currentStepIndex];
        std::vector<int> partialOrder(traversalOrder.begin(),
                                       traversalOrder.begin() + currentStepIndex + 1);
        callback(vertex, partialOrder);
        currentStepIndex++;
        timer->start(500); // Задержка 500 мс между шагами
    } else {
        running = false;
        emit finished();
    }
}

void AlgorithmRunner::stop() {
    running = false;
    if (timer) timer->stop();
}

bool AlgorithmRunner::isRunning() const {
    return running;
}

std::vector<int> AlgorithmRunner::getBFSTraversal(const Graph& graph, int startVertex) {
    std::vector<int> order;

    // Проверка: существует ли начальная вершина
    if (!graph.hasVertex(startVertex)) {
        return order; // возвращаем пустой вектор
    }

    std::queue<int> q;
    std::unordered_set<int> visited;

    q.push(startVertex);
    visited.insert(startVertex);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        order.push_back(current);

        auto neighbors = graph.getNeighbors(current);
        for (int neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    return order;
}

std::vector<int> AlgorithmRunner::getDFSTraversal(const Graph& graph, int startVertex) {
    std::vector<int> order;

    // Проверка: существует ли начальная вершина
    if (!graph.hasVertex(startVertex)) {
        return order; // возвращаем пустой вектор
    }

    std::stack<int> s;
    std::unordered_set<int> visited;

    s.push(startVertex);

    while (!s.empty()) {
        int current = s.top();
        s.pop();

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            order.push_back(current);

            auto neighbors = graph.getNeighbors(current);
            // Добавляем соседей в обратном порядке для правильного порядка DFS
            for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
                if (visited.find(*it) == visited.end()) {
                    s.push(*it);
                }
            }
        }
    }

    return order;
}