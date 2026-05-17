#include <gtest/gtest.h>
#include "GraphLayout.h"
#include "Graph.h"
#include <cmath>

// ==================== Тесты для GraphLayout ====================

class GraphLayoutTest : public ::testing::Test {
protected:
    void SetUp() override {
        layout = GraphLayout();
    }

    GraphLayout layout;
};

TEST_F(GraphLayoutTest, EmptyGraphLayout) {
    Graph g;
    layout.untangle(g);
    auto positions = layout.getVertexPositions();
    EXPECT_TRUE(positions.empty());
}

TEST_F(GraphLayoutTest, SingleVertexLayout) {
    Graph g;
    g.addVertex(1);
    layout.untangle(g);
    auto positions = layout.getVertexPositions();

    EXPECT_EQ(positions.size(), 1);
    EXPECT_TRUE(positions.find(1) != positions.end());

    // Проверяем, что позиция в разумных пределах
    QPointF pos = positions[1];
    EXPECT_GE(pos.x(), 0);
    EXPECT_LE(pos.x(), 800);
    EXPECT_GE(pos.y(), 0);
    EXPECT_LE(pos.y(), 600);
}

TEST_F(GraphLayoutTest, TwoVerticesLayout) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    layout.untangle(g);
    auto positions = layout.getVertexPositions();

    EXPECT_EQ(positions.size(), 2);

    QPointF p1 = positions[1];
    QPointF p2 = positions[2];

    // Расстояние между вершинами не должно быть нулевым
    double distance = std::hypot(p1.x() - p2.x(), p1.y() - p2.y());
    EXPECT_GT(distance, 10.0);
}

TEST_F(GraphLayoutTest, CircularLayoutForThreeVertices) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 1);

    // Применяем только круговую раскладку через untangle
    layout.untangle(g);
    auto positions = layout.getVertexPositions();

    EXPECT_EQ(positions.size(), 3);

    // Проверяем, что все вершины имеют разные позиции
    QPointF p1 = positions[1];
    QPointF p2 = positions[2];
    QPointF p3 = positions[3];

    EXPECT_FALSE(p1 == p2);
    EXPECT_FALSE(p2 == p3);
    EXPECT_FALSE(p1 == p3);
}

TEST_F(GraphLayoutTest, SetAndGetVertexPosition) {
    QPointF testPos(100, 200);
    layout.setVertexPosition(5, testPos);

    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 1);
    EXPECT_EQ(positions[5], testPos);
}

TEST_F(GraphLayoutTest, UpdateVertexPosition) {
    layout.setVertexPosition(1, QPointF(10, 10));
    layout.setVertexPosition(1, QPointF(50, 50));

    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions[1], QPointF(50, 50));
}

TEST_F(GraphLayoutTest, ResetLayout) {
    layout.setVertexPosition(1, QPointF(100, 100));
    layout.setVertexPosition(2, QPointF(200, 200));
    layout.reset();

    auto positions = layout.getVertexPositions();
    EXPECT_TRUE(positions.empty());
}

TEST_F(GraphLayoutTest, MultipleVerticesLayout) {
    Graph g;
    for (int i = 1; i <= 10; i++) g.addVertex(i);
    for (int i = 1; i <= 9; i++) g.addEdge(i, i + 1);

    layout.untangle(g);
    auto positions = layout.getVertexPositions();

    EXPECT_EQ(positions.size(), 10);

    // Проверяем, что все вершины имеют уникальные позиции
    std::unordered_set<std::string> uniquePositions;
    for (const auto& pair : positions) {
        std::string posKey = std::to_string(pair.second.x()) + "," + std::to_string(pair.second.y());
        uniquePositions.insert(posKey);
    }
    EXPECT_EQ(uniquePositions.size(), 10);
}

TEST_F(GraphLayoutTest, LayoutDoesNotCrashWithLargeGraph) {
    Graph g;
    // Создаём граф с 50 вершинами
    for (int i = 1; i <= 50; i++) {
        g.addVertex(i);
    }
    for (int i = 1; i <= 49; i++) {
        g.addEdge(i, i + 1);
    }

    // Должно выполниться без ошибок
    EXPECT_NO_THROW(layout.untangle(g));

    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 50);
}

TEST_F(GraphLayoutTest, VertexPositionsWithinBounds) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);

    layout.untangle(g);
    auto positions = layout.getVertexPositions();

    for (const auto& pair : positions) {
        QPointF pos = pair.second;
        // Проверяем, что позиции в разумных пределах (не уходят в бесконечность)
        EXPECT_FALSE(std::isnan(pos.x()));
        EXPECT_FALSE(std::isnan(pos.y()));
        EXPECT_GT(pos.x(), -10000);
        EXPECT_LT(pos.x(), 10000);
        EXPECT_GT(pos.y(), -10000);
        EXPECT_LT(pos.y(), 10000);
    }
}

TEST_F(GraphLayoutTest, GraphWithEdgesOnlyLayout) {
    Graph g;
    for (int i = 1; i <= 4; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 1);

    layout.untangle(g);
    auto positions = layout.getVertexPositions();

    EXPECT_EQ(positions.size(), 4);

    // Проверяем, что рёбра не нулевой длины
    QPointF p1 = positions[1];
    QPointF p2 = positions[2];
    QPointF p3 = positions[3];
    QPointF p4 = positions[4];

    EXPECT_GT(std::hypot(p1.x() - p2.x(), p1.y() - p2.y()), 5.0);
    EXPECT_GT(std::hypot(p2.x() - p3.x(), p2.y() - p3.y()), 5.0);
    EXPECT_GT(std::hypot(p3.x() - p4.x(), p3.y() - p4.y()), 5.0);
    EXPECT_GT(std::hypot(p4.x() - p1.x(), p4.y() - p1.y()), 5.0);
}