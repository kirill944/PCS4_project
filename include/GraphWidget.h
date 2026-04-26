#ifndef GRAPH_WIDGET_H
#define GRAPH_WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include "GraphScene.h"
#include "GraphLayout.h"
#include "AlgorithmRunner.h"

class GraphWidget : public QWidget {
    Q_OBJECT  // Добавлен макрос Q_OBJECT

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
    void clearAll();

    private slots:
        void onAlgorithmStep(int currentVertex, const std::vector<int>& order);
    void onAlgorithmFinished();

private:
    Graph graph;
    GraphScene* scene;
    QGraphicsView* view;
    GraphLayout layout;
    AlgorithmRunner* algorithmRunner;

    int nextVertexId;
    int selectedVertexForEdge;

    void updateVisualization(int highlightedVertex = -1,
                             const std::unordered_set<int>& visitedSet = {});
    void setupUI();
};

#endif // GRAPH_WIDGET_H