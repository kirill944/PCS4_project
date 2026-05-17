#ifndef GRAPH_LAYOUT_H
#define GRAPH_LAYOUT_H

#include "Graph.h"
#include <QPointF>
#include <unordered_map>
#include <vector>
#include <random>

class GraphLayout {
public:
    GraphLayout();
    ~GraphLayout();

    void untangle(const Graph& graph);
    std::unordered_map<int, QPointF> getVertexPositions() const;
    void setVertexPosition(int vertex, const QPointF& pos);
    void reset();

private:
    std::unordered_map<int, QPointF> positions;

    void forceDirectedLayout(const Graph& graph);
    void circularLayout(const Graph& graph);

    QPointF repulsiveForce(const QPointF& p1, const QPointF& p2, double k);
    QPointF attractiveForce(const QPointF& p1, const QPointF& p2, double k);
};

#endif // GRAPH_LAYOUT_H