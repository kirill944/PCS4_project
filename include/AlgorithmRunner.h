#ifndef ALGORITHM_RUNNER_H
#define ALGORITHM_RUNNER_H

#include "Graph.h"
#include <QObject>
#include <QTimer>
#include <vector>
#include <functional>
#include <queue>
#include <stack>
#include <unordered_set>

class AlgorithmRunner : public QObject {
    Q_OBJECT

public:
    explicit AlgorithmRunner(QObject* parent = nullptr);
    ~AlgorithmRunner();

    void runBFS(const Graph& graph, int startVertex,
                std::function<void(int, const std::vector<int>&)> stepCallback);
    void runDFS(const Graph& graph, int startVertex,
                std::function<void(int, const std::vector<int>&)> stepCallback);
    void stop();
    bool isRunning() const;

    std::vector<int> getBFSTraversal(const Graph& graph, int startVertex);
    std::vector<int> getDFSTraversal(const Graph& graph, int startVertex);

    signals:
        void finished();
    void stepPerformed(int currentVertex, const std::vector<int>& order);

private:
    QTimer* timer;
    bool running;
    int currentStepIndex;
    std::vector<int> traversalOrder;

    void executeNextStep(std::function<void(int, const std::vector<int>&)> callback);
};

std::vector<int> getBFSTraversal(const Graph& graph, int startVertex);
std::vector<int> getDFSTraversal(const Graph& graph, int startVertex);

#endif // ALGORITHM_RUNNER_H