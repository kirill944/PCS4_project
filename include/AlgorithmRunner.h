#ifndef ALGORITHM_RUNNER_H
#define ALGORITHM_RUNNER_H

#include "Graph.h"
#include <QObject>
#include <QTimer>
#include <vector>
#include <functional>

class AlgorithmRunner : public QObject {
    Q_OBJECT  // Этот макрос требует MOC

public:
    explicit AlgorithmRunner(QObject* parent = nullptr);
    ~AlgorithmRunner();

    void runBFS(int startVertex, std::function<void(int, const std::vector<int>&)> stepCallback);
    void runDFS(int startVertex, std::function<void(int, const std::vector<int>&)> stepCallback);
    void stop();
    bool isRunning() const;

    signals:
        void finished();  // Сигнал - требует MOC
    void stepPerformed(int currentVertex, const std::vector<int>& order);

private:
    QTimer* timer;
    bool running;
    std::function<void(int, const std::vector<int>&)> currentCallback;

    void bfsStep(int startVertex, std::function<void(int, const std::vector<int>&)> callback);
    void dfsStep(int startVertex, std::function<void(int, const std::vector<int>&)> callback);
};

#endif // ALGORITHM_RUNNER_H