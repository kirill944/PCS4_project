#ifndef GRAPH_LAYOUT_H
#define GRAPH_LAYOUT_H

#include "Graph.h"
#include <QPointF>
#include <unordered_map>
#include <vector>

class GraphLayout {
public:
    GraphLayout();
    ~GraphLayout();

    // Запуск алгоритма распутывания (убирание пересечений)
    void untangle(const Graph& graph);

    // Получение позиций вершин
    std::unordered_map<int, QPointF> getVertexPositions() const;

    // Ручная установка позиции вершины
    void setVertexPosition(int vertex, const QPointF& pos);

    // Сброс позиций
    void reset();

private:
    std::unordered_map<int, QPointF> positions;

    // Алгоритм Force-Directed (Fruchterman-Reingold)
    void forceDirectedLayout(const Graph& graph);

    // Вычисление силы отталкивания между вершинами
    QPointF repulsiveForce(const QPointF& p1, const QPointF& p2, double k);

    // Вычисление силы притяжения по ребру
    QPointF attractiveForce(const QPointF& p1, const QPointF& p2, double k);
};

#endif // GRAPH_LAYOUT_H