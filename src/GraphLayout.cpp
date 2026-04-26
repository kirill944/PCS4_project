#include "GraphLayout.h"
#include <cmath>
#include <random>
#include <QDebug>

GraphLayout::GraphLayout() {}

GraphLayout::~GraphLayout() {}

void GraphLayout::untangle(const Graph& graph) {
    forceDirectedLayout(graph);
}

void GraphLayout::forceDirectedLayout(const Graph& graph) {
    // Заглушка: просто размещаем вершины по кругу
    auto vertices = graph.getVertices();
    int n = vertices.size();
    if (n == 0) return;

    double angleStep = 2 * M_PI / n;
    double radius = 200.0;

    for (size_t i = 0; i < vertices.size(); ++i) {
        double angle = i * angleStep;
        double x = 400 + radius * cos(angle);
        double y = 300 + radius * sin(angle);
        positions[vertices[i]] = QPointF(x, y);
    }
}

QPointF GraphLayout::repulsiveForce(const QPointF& p1, const QPointF& p2, double k) {
    // Заглушка
    return QPointF(0, 0);
}

QPointF GraphLayout::attractiveForce(const QPointF& p1, const QPointF& p2, double k) {
    // Заглушка
    return QPointF(0, 0);
}

std::unordered_map<int, QPointF> GraphLayout::getVertexPositions() const {
    return positions;
}

void GraphLayout::setVertexPosition(int vertex, const QPointF& pos) {
    positions[vertex] = pos;
}

void GraphLayout::reset() {
    positions.clear();
}