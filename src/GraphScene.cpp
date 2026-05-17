#include "GraphScene.h"
#include <QPen>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

const QColor GraphScene::DEFAULT_VERTEX_COLOR = Qt::blue;
const QColor GraphScene::HIGHLIGHT_VERTEX_COLOR = Qt::red;
const QColor GraphScene::VISITED_VERTEX_COLOR = Qt::green;
const QColor GraphScene::EDGE_COLOR = Qt::black;

GraphScene::GraphScene(QObject* parent)
    : QGraphicsScene(parent), draggedVertex(-1), isDraggingEnabled(true), currentGraph(nullptr) {
    setSceneRect(0, 0, 800, 600);
}

GraphScene::~GraphScene() {
    clearScene();
}

void GraphScene::updateGraph(const Graph& graph,
                             const std::unordered_map<int, QPointF>& positions,
                             int highlightedVertex,
                             const std::unordered_set<int>& visitedSet) {
    // Сохраняем текущие граф и позиции для обновления рёбер
    currentGraph = &graph;
    currentPositions = positions;

    // Полностью очищаем сцену перед обновлением
    clearScene();

    auto vertices = graph.getVertices();

    // Создаём рёбра сначала (чтобы они были под вершинами)
    for (int v : vertices) {
        if (positions.find(v) == positions.end()) continue;
        auto neighbors = graph.getNeighbors(v);
        for (int neighbor : neighbors) {
            if (v < neighbor) { // Чтобы не рисовать дважды
                if (positions.find(v) != positions.end() &&
                    positions.find(neighbor) != positions.end()) {
                    QPointF p1 = positions.at(v);
                    QPointF p2 = positions.at(neighbor);
                    QGraphicsLineItem* line = addLine(p1.x(), p1.y(),
                                                       p2.x(), p2.y(),
                                                       QPen(EDGE_COLOR, 2));
                    edgeItems.push_back({line, v, neighbor});
                }
            }
        }
    }

    // Создаём вершины
    for (int v : vertices) {
        if (positions.find(v) == positions.end()) continue;
        QPointF pos = positions.at(v);

        QColor color = DEFAULT_VERTEX_COLOR;
        if (v == highlightedVertex) {
            color = HIGHLIGHT_VERTEX_COLOR;
        } else if (visitedSet.find(v) != visitedSet.end()) {
            color = VISITED_VERTEX_COLOR;
        }

        // Создаём круг
        QGraphicsEllipseItem* circle = addEllipse(pos.x() - 20, pos.y() - 20,
                                                  40, 40,
                                                  QPen(Qt::black, 2),
                                                  QBrush(color));
        circle->setData(0, v);

        // Создаём текст
        QGraphicsTextItem* text = addText(QString::number(v));
        QFont font("Arial", 12, QFont::Bold);
        text->setFont(font);
        text->setDefaultTextColor(Qt::white);

        // Центрируем текст
        QRectF textRect = text->boundingRect();
        double textX = pos.x() - textRect.width() / 2;
        double textY = pos.y() - textRect.height() / 2;
        text->setPos(textX, textY);

        vertexItems[v] = {circle, text, QPointF(), false, v};
    }
}

void GraphScene::updateVertexPosition(int vertex, const QPointF& newPos) {
    auto it = vertexItems.find(vertex);
    if (it != vertexItems.end()) {
        VertexItem& item = it->second;

        // Обновляем позицию круга
        item.circle->setPos(newPos.x() - 20, newPos.y() - 20);

        // Обновляем позицию текста с центрированием
        QRectF textRect = item.text->boundingRect();
        double textX = newPos.x() - textRect.width() / 2 + 380;
        double textY = newPos.y() - textRect.height() / 2 + 280;
        item.text->setPos(textX, textY);

        // Обновляем позицию в кэше
        currentPositions[vertex] = newPos;

        // Обновляем связанные рёбра
        if (currentGraph) {
            auto neighbors = currentGraph->getNeighbors(vertex);
            for (int neighbor : neighbors) {
                updateSingleEdge(vertex, neighbor);
            }
        }
    }
}

void GraphScene::updateEdgePositions(const Graph& graph,
                                      const std::unordered_map<int, QPointF>& positions) {
    currentPositions = positions;

    // Обновляем все рёбра
    for (auto& edge : edgeItems) {
        auto itFrom = positions.find(edge.from);
        auto itTo = positions.find(edge.to);
        if (itFrom != positions.end() && itTo != positions.end()) {
            QPointF p1 = itFrom->second;
            QPointF p2 = itTo->second;
            edge.line->setLine(p1.x(), p1.y(), p2.x(), p2.y());
        }
    }
}

void GraphScene::updateSingleEdge(int from, int to) {
    for (auto& edge : edgeItems) {
        if ((edge.from == from && edge.to == to) ||
            (edge.from == to && edge.to == from)) {
            auto itFrom = currentPositions.find(from);
            auto itTo = currentPositions.find(to);
            if (itFrom != currentPositions.end() && itTo != currentPositions.end()) {
                QPointF p1 = itFrom->second;
                QPointF p2 = itTo->second;
                edge.line->setLine(p1.x(), p1.y(), p2.x(), p2.y());
            }
            break;
        }
    }
}

void GraphScene::clearScene() {
    // Очищаем вершины
    for (auto& pair : vertexItems) {
        delete pair.second.circle;
        delete pair.second.text;
    }
    vertexItems.clear();

    // Очищаем рёбра
    for (const auto& edge : edgeItems) {
        delete edge.line;
    }
    edgeItems.clear();

    // Очищаем сцену
    clear();
}

void GraphScene::highlightVertex(int vertex, bool highlight) {
    if (vertexItems.find(vertex) != vertexItems.end()) {
        QColor color = highlight ? HIGHLIGHT_VERTEX_COLOR : DEFAULT_VERTEX_COLOR;
        vertexItems[vertex].circle->setBrush(QBrush(color));
    }
}

void GraphScene::setVertexDraggable(bool draggable) {
    isDraggingEnabled = draggable;
}

std::unordered_map<int, QPointF> GraphScene::getDraggedPositions() const {
    std::unordered_map<int, QPointF> positions;
    for (const auto& pair : vertexItems) {
        QPointF pos = pair.second.circle->pos();
        positions[pair.first] = QPointF(pos.x() + 20, pos.y() + 20);
    }
    return positions;
}

GraphScene::VertexItem* GraphScene::findVertexItemAt(const QPointF& pos) {
    for (auto& pair : vertexItems) {
        QRectF rect = pair.second.circle->sceneBoundingRect();
        if (rect.contains(pos)) {
            return &pair.second;
        }
    }
    return nullptr;
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (!isDraggingEnabled) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

    VertexItem* item = findVertexItemAt(event->scenePos());
    if (item) {
        draggedVertex = item->id;
        item->dragStartPos = event->scenePos();
        item->isDragging = true;
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (!isDraggingEnabled || draggedVertex == -1) {
        QGraphicsScene::mouseMoveEvent(event);
        return;
    }

    auto it = vertexItems.find(draggedVertex);
    if (it != vertexItems.end()) {
        VertexItem& item = it->second;
        if (item.isDragging) {
            QPointF currentPos = event->scenePos();
            QPointF delta = currentPos - item.dragStartPos;

            // Получаем текущую позицию вершины
            QPointF oldCenter = QPointF(item.circle->pos().x() + 20, item.circle->pos().y() + 20);
            QPointF newCenter = oldCenter + delta;

            // Обновляем позицию вершины
            updateVertexPosition(draggedVertex, newCenter);

            item.dragStartPos = currentPos;

            emit vertexMoved(draggedVertex, newCenter);
        }
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (draggedVertex != -1) {
        auto it = vertexItems.find(draggedVertex);
        if (it != vertexItems.end()) {
            it->second.isDragging = false;
        }
    }
    draggedVertex = -1;
    QGraphicsScene::mouseReleaseEvent(event);
}