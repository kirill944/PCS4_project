#include "AlgorithmRunner.h"
#include <queue>
#include <stack>
#include <unordered_set>

AlgorithmRunner::AlgorithmRunner(QObject* parent)
    : QObject(parent), timer(new QTimer(this)), running(false) {
    timer->setSingleShot(false);
}

AlgorithmRunner::~AlgorithmRunner() {
    stop();
    delete timer;
}

void AlgorithmRunner::runBFS(int startVertex,
                             std::function<void(int, const std::vector<int>&)> stepCallback) {
    // Заглушка: просто эмулируем BFS
    running = true;
    std::queue<int> q;
    std::unordered_set<int> visited;
    std::vector<int> order;

    q.push(startVertex);
    visited.insert(startVertex);

    while (!q.empty() && running) {
        int current = q.front();
        q.pop();
        order.push_back(current);

        // Эмуляция шага визуализации
        stepCallback(current, order);

        // Здесь должен быть код для получения соседей
        // Но т.к. Graph не передан, это заглушка
        break;
    }

    running = false;
    emit finished();
}

void AlgorithmRunner::runDFS(int startVertex,
                             std::function<void(int, const std::vector<int>&)> stepCallback) {
    // Заглушка: аналогично BFS
    running = true;
    std::stack<int> s;
    std::unordered_set<int> visited;
    std::vector<int> order;

    s.push(startVertex);

    while (!s.empty() && running) {
        int current = s.top();
        s.pop();

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            order.push_back(current);
            stepCallback(current, order);
        }
        break;
    }

    running = false;
    emit finished();
}

void AlgorithmRunner::stop() {
    running = false;
}

bool AlgorithmRunner::isRunning() const {
    return running;
}

void AlgorithmRunner::bfsStep(int startVertex,
                              std::function<void(int, const std::vector<int>&)> callback) {
    // Заглушка
}

void AlgorithmRunner::dfsStep(int startVertex,
                              std::function<void(int, const std::vector<int>&)> callback) {
    // Заглушка
}