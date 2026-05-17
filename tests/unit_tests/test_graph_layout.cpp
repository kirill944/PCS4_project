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


TEST_F(GraphLayoutTest, Untangle_1Vertex) {
    Graph g;
    g.addVertex(1);
    layout.untangle(g);
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 1);
}

TEST_F(GraphLayoutTest, Untangle_2Vertices) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    layout.untangle(g);
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 2);
    QPointF p1 = positions[1];
    QPointF p2 = positions[2];
    EXPECT_GT(std::hypot(p1.x() - p2.x(), p1.y() - p2.y()), 10.0);
}

TEST_F(GraphLayoutTest, Untangle_3Vertices_Line) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    layout.untangle(g);
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 3);
}

TEST_F(GraphLayoutTest, Untangle_3Vertices_Triangle) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 1);
    layout.untangle(g);
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 3);
}

TEST_F(GraphLayoutTest, Untangle_4Vertices_Square) {
    Graph g;
    for (int i = 1; i <= 4; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 1);
    layout.untangle(g);
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 4);
}

TEST_F(GraphLayoutTest, Untangle_5Vertices_Path) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    for (int i = 1; i <= 4; i++) g.addEdge(i, i + 1);
    layout.untangle(g);
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 5);
}

TEST_F(GraphLayoutTest, Untangle_5Vertices_Star) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    for (int i = 2; i <= 5; i++) g.addEdge(1, i);
    layout.untangle(g);
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 5);
}

TEST_F(GraphLayoutTest, SetAndGet_1Vertex) {
    layout.setVertexPosition(1, QPointF(100, 100));
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 1);
    EXPECT_EQ(positions[1], QPointF(100, 100));
}

TEST_F(GraphLayoutTest, SetAndGet_2Vertices) {
    layout.setVertexPosition(1, QPointF(100, 100));
    layout.setVertexPosition(2, QPointF(200, 200));
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 2);
    EXPECT_EQ(positions[1], QPointF(100, 100));
    EXPECT_EQ(positions[2], QPointF(200, 200));
}

TEST_F(GraphLayoutTest, SetAndGet_3Vertices) {
    for (int i = 1; i <= 3; i++) {
        layout.setVertexPosition(i, QPointF(i * 100, i * 100));
    }
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 3);
}

TEST_F(GraphLayoutTest, SetAndGet_4Vertices) {
    for (int i = 1; i <= 4; i++) {
        layout.setVertexPosition(i, QPointF(i * 50, i * 50));
    }
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 4);
}

TEST_F(GraphLayoutTest, SetAndGet_5Vertices) {
    for (int i = 1; i <= 5; i++) {
        layout.setVertexPosition(i, QPointF(i * 80, i * 80));
    }
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions.size(), 5);
}

TEST_F(GraphLayoutTest, UpdatePosition) {
    layout.setVertexPosition(1, QPointF(10, 10));
    layout.setVertexPosition(1, QPointF(50, 50));
    auto positions = layout.getVertexPositions();
    EXPECT_EQ(positions[1], QPointF(50, 50));
}

TEST_F(GraphLayoutTest, Reset_Empty) {
    layout.reset();
    auto positions = layout.getVertexPositions();
    EXPECT_TRUE(positions.empty());
}

TEST_F(GraphLayoutTest, Reset_With1Vertex) {
    layout.setVertexPosition(1, QPointF(100, 100));
    layout.reset();
    auto positions = layout.getVertexPositions();
    EXPECT_TRUE(positions.empty());
}

TEST_F(GraphLayoutTest, Reset_With3Vertices) {
    for (int i = 1; i <= 3; i++) {
        layout.setVertexPosition(i, QPointF(i * 100, i * 100));
    }
    layout.reset();
    auto positions = layout.getVertexPositions();
    EXPECT_TRUE(positions.empty());
}

TEST_F(GraphLayoutTest, Reset_With5Vertices) {
    for (int i = 1; i <= 5; i++) {
        layout.setVertexPosition(i, QPointF(i * 100, i * 100));
    }
    layout.reset();
    auto positions = layout.getVertexPositions();
    EXPECT_TRUE(positions.empty());
}

TEST_F(GraphLayoutTest, Reset_Twice) {
    layout.setVertexPosition(1, QPointF(100, 100));
    layout.reset();
    layout.reset();  // Повторный сброс
    auto positions = layout.getVertexPositions();
    EXPECT_TRUE(positions.empty());
}

