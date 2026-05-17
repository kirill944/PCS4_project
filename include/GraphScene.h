#ifndef GRAPH_SCENE_H
#define GRAPH_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <unordered_map>
#include <unordered_set>
#include "Graph.h"

class GraphScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit GraphScene(QObject* parent = nullptr);
    ~GraphScene();

    void updateGraph(const Graph& graph,
                     const std::unordered_map<int, QPointF>& positions,
                     int highlightedVertex = -1,
                     const std::unordered_set<int>& visitedSet = {});
    void clearScene();
    void highlightVertex(int vertex, bool highlight);
    void setVertexDraggable(bool draggable);
    std::unordered_map<int, QPointF> getDraggedPositions() const;
    void updateEdgePositions(const Graph& graph,
                             const std::unordered_map<int, QPointF>& positions);
    void updateVertexPosition(int vertex, const QPointF& newPos);

signals:
    void vertexClicked(int vertex);
    void vertexMoved(int vertex, const QPointF& pos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    struct VertexItem {
        QGraphicsEllipseItem* circle;
        QGraphicsTextItem* text;
        QPointF dragStartPos;
        bool isDragging;
        int id;
    };

    struct EdgeItem {
        QGraphicsLineItem* line;
        int from;
        int to;
    };

    std::unordered_map<int, VertexItem> vertexItems;
    std::vector<EdgeItem> edgeItems;

    int draggedVertex;
    bool isDraggingEnabled;
    const Graph* currentGraph;
    std::unordered_map<int, QPointF> currentPositions;

    static const QColor DEFAULT_VERTEX_COLOR;
    static const QColor HIGHLIGHT_VERTEX_COLOR;
    static const QColor VISITED_VERTEX_COLOR;
    static const QColor EDGE_COLOR;

    VertexItem* findVertexItemAt(const QPointF& pos);
    void updateSingleEdge(int from, int to);
};

#endif // GRAPH_SCENE_H