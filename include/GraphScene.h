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
    Q_OBJECT  // Добавлен макрос Q_OBJECT

public:
    explicit GraphScene(QObject* parent = nullptr);
    ~GraphScene();

    void updateGraph(const Graph& graph,
                     const std::unordered_map<int, QPointF>& positions,
                     int highlightedVertex = -1,
                     const std::unordered_set<int>& visitedSet = {});
    void clearScene();
    void highlightVertex(int vertex, bool highlight);

    signals:
        void vertexClicked(int vertex);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    struct VertexItem {
        QGraphicsEllipseItem* circle;
        QGraphicsTextItem* text;
    };

    std::unordered_map<int, VertexItem> vertexItems;
    std::vector<QGraphicsLineItem*> edgeItems;

    static const QColor DEFAULT_VERTEX_COLOR;
    static const QColor HIGHLIGHT_VERTEX_COLOR;
    static const QColor VISITED_VERTEX_COLOR;
    static const QColor EDGE_COLOR;
};

#endif // GRAPH_SCENE_H