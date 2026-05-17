#ifndef GRAPH_WIDGET_H
#define GRAPH_WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QTimer>
#include "GraphScene.h"
#include "GraphLayout.h"
#include "AlgorithmRunner.h"

class GraphWidget : public QWidget {
    Q_OBJECT

public:
    explicit GraphWidget(QWidget* parent = nullptr);
    ~GraphWidget();

    void addVertexManually(int id);
    void removeVertexManually(int id);
    void addEdgeManually(int from, int to);
    void removeEdgeManually(int from, int to);
    void untangleGraph();
    void startBFS(int startVertex);
    void startDFS(int startVertex);
    void generateRandomGraph(int vertexCount, int edgeCount);
    void saveGraphToFile(const QString& filename);
    void loadGraphFromFile(const QString& filename);
    void clearAll();
    void nextAlgorithmStep();

    QGraphicsView* getView() const { return view; }

    private slots:
        void onAlgorithmStep(int currentVertex, const std::vector<int>& order);
    void onAlgorithmFinished();
    void onVertexMoved(int vertex, const QPointF& pos);
    void onAutoUntangle();

private:
    Graph graph;
    GraphScene* scene;
    QGraphicsView* view;
    GraphLayout layout;
    AlgorithmRunner* algorithmRunner;
    QTimer* untangleTimer;

    int nextVertexId;
    int selectedVertexForEdge;
    bool isAlgorithmRunning;
    bool isStepByStepMode;
    std::vector<int> currentTraversalOrder;
    int currentStepIndex;

    void updateVisualization(int highlightedVertex = -1,
                         const std::unordered_set<int>& visitedSet = {});
    void setupUI();
    void syncPositionsFromScene();
    void finishAlgorithm();  // Добавьте эту строку

    friend class MainWindow;
};

#endif // GRAPH_WIDGET_H