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
    auto vertices = graph.getVertices();
    int n = vertices.size();
    if (n == 0) return;

    // Начальная круговая раскладка
    circularLayout(graph);

    if (n <= 1) return;

    // Параметры силового алгоритма
    double width = 700.0;
    double height = 500.0;
    double k = sqrt(width * height / n);
    double temperature = width / 10.0;
    double cooling = 0.95;
    int iterations = 50;

    std::vector<QPointF> forces(n);
    std::vector<int> vertexList = vertices;

    for (int iter = 0; iter < iterations; ++iter) {
        // Обнуляем силы
        for (int i = 0; i < n; ++i) {
            forces[i] = QPointF(0, 0);
        }

        // Вычисляем силы отталкивания
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                QPointF p1 = positions[vertexList[i]];
                QPointF p2 = positions[vertexList[j]];
                QPointF force = repulsiveForce(p1, p2, k);
                forces[i] -= force;
                forces[j] += force;
            }
        }

        // Вычисляем силы притяжения по рёбрам
        for (int i = 0; i < n; ++i) {
            int v = vertexList[i];
            auto neighbors = graph.getNeighbors(v);
            for (int neighbor : neighbors) {
                int j = -1;
                for (int idx = 0; idx < n; ++idx) {
                    if (vertexList[idx] == neighbor) {
                        j = idx;
                        break;
                    }
                }
                if (j != -1 && i < j) {
                    QPointF p1 = positions[v];
                    QPointF p2 = positions[neighbor];
                    QPointF force = attractiveForce(p1, p2, k);
                    forces[i] += force;
                    forces[j] -= force;
                }
            }
        }

        // Применяем силы и ограничиваем перемещение температурой
        for (int i = 0; i < n; ++i) {
            double dx = forces[i].x();
            double dy = forces[i].y();
            double length = sqrt(dx * dx + dy * dy);
            if (length > 0) {
                dx = dx / length * std::min(length, temperature);
                dy = dy / length * std::min(length, temperature);
            }

            QPointF newPos = positions[vertexList[i]] + QPointF(dx, dy);
            newPos.setX(std::max(50.0, std::min(width + 50.0, newPos.x())));
            newPos.setY(std::max(50.0, std::min(height + 50.0, newPos.y())));
            positions[vertexList[i]] = newPos;
        }

        temperature *= cooling;
    }
}

void GraphLayout::circularLayout(const Graph& graph) {
    auto vertices = graph.getVertices();
    int n = vertices.size();
    if (n == 0) return;

    double centerX = 400.0;
    double centerY = 300.0;
    double radius = std::min(centerX, centerY) - 50;
    double angleStep = 2 * M_PI / n;

    for (size_t i = 0; i < vertices.size(); ++i) {
        double angle = i * angleStep;
        double x = centerX + radius * cos(angle);
        double y = centerY + radius * sin(angle);
        positions[vertices[i]] = QPointF(x, y);
    }
}

QPointF GraphLayout::repulsiveForce(const QPointF& p1, const QPointF& p2, double k) {
    QPointF delta = p2 - p1;
    double distance = sqrt(delta.x() * delta.x() + delta.y() * delta.y());
    if (distance < 0.1) distance = 0.1;
    double forceMagnitude = k * k / distance;
    return delta / distance * forceMagnitude;
}

QPointF GraphLayout::attractiveForce(const QPointF& p1, const QPointF& p2, double k) {
    QPointF delta = p2 - p1;
    double distance = sqrt(delta.x() * delta.x() + delta.y() * delta.y());
    if (distance < 0.1) distance = 0.1;
    double forceMagnitude = distance * distance / k;
    return delta / distance * forceMagnitude;
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