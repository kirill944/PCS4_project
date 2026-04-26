#include "GraphScene.h"
#include <QPen>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>

const QColor GraphScene::DEFAULT_VERTEX_COLOR = Qt::blue;
const QColor GraphScene::HIGHLIGHT_VERTEX_COLOR = Qt::red;
const QColor GraphScene::VISITED_VERTEX_COLOR = Qt::green;
const QColor GraphScene::EDGE_COLOR = Qt::black;

GraphScene::GraphScene(QObject* parent)
    : QGraphicsScene(parent) {
    setSceneRect(0, 0, 800, 600);
}

GraphScene::~GraphScene() {
    clearScene();
}

void GraphScene::updateGraph(const Graph& graph,
                             const std::unordered_map<int, QPointF>& positions,
                             int highlightedVertex,
                             const std::unordered_set<int>& visitedSet) {
    clearScene();

    // Рисуем рёбра
    auto vertices = graph.getVertices();
    for (int v : vertices) {
        if (positions.find(v) == positions.end()) continue;
        auto neighbors = graph.getNeighbors(v);
        for (int neighbor : neighbors) {
            if (v < neighbor) { // Чтобы не рисовать дважды
                if (positions.find(neighbor) != positions.end()) {
                    QPointF p1 = positions.at(v);
                    QPointF p2 = positions.at(neighbor);
                    QGraphicsLineItem* line = addLine(p1.x(), p1.y(),
                                                       p2.x(), p2.y(),
                                                       QPen(EDGE_COLOR, 2));
                    edgeItems.push_back(line);
                }
            }
        }
    }

    // Рисуем вершины
    for (int v : vertices) {
        if (positions.find(v) == positions.end()) continue;
        QPointF pos = positions.at(v);

        QColor color = DEFAULT_VERTEX_COLOR;
        if (v == highlightedVertex) {
            color = HIGHLIGHT_VERTEX_COLOR;
        } else if (visitedSet.find(v) != visitedSet.end()) {
            color = VISITED_VERTEX_COLOR;
        }

        QGraphicsEllipseItem* circle = addEllipse(pos.x() - 20, pos.y() - 20,
                                                  40, 40,
                                                  QPen(Qt::black, 2),
                                                  QBrush(color));
        QGraphicsTextItem* text = addText(QString::number(v));
        text->setPos(pos.x() - 10, pos.y() - 10);

        vertexItems[v] = {circle, text};
    }
}

void GraphScene::clearScene() {
    for (auto& pair : vertexItems) {
        delete pair.second.circle;
        delete pair.second.text;
    }
    vertexItems.clear();

    for (QGraphicsLineItem* line : edgeItems) {
        delete line;
    }
    edgeItems.clear();

    clear();
}

void GraphScene::highlightVertex(int vertex, bool highlight) {
    if (vertexItems.find(vertex) != vertexItems.end()) {
        QColor color = highlight ? HIGHLIGHT_VERTEX_COLOR : DEFAULT_VERTEX_COLOR;
        vertexItems[vertex].circle->setBrush(QBrush(color));
    }
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    // Заглушка: определяем клик по вершине
    QGraphicsScene::mousePressEvent(event);
}