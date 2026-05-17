#include <gtest/gtest.h>
#include "Graph.h"

// ==================== Тесты для Graph ====================

TEST(GraphTest, AddVertex) {
    Graph g;
    g.addVertex(1);
    EXPECT_TRUE(g.hasVertex(1));
    EXPECT_EQ(g.vertexCount(), 1);
}

TEST(GraphTest, AddDuplicateVertex) {
    Graph g;
    g.addVertex(1);
    g.addVertex(1);
    EXPECT_EQ(g.vertexCount(), 1);
}

TEST(GraphTest, RemoveVertex) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    g.removeVertex(1);
    EXPECT_FALSE(g.hasVertex(1));
    EXPECT_FALSE(g.hasEdge(1, 2));
    EXPECT_EQ(g.vertexCount(), 1);
}

TEST(GraphTest, AddEdge) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    EXPECT_TRUE(g.hasEdge(1, 2));
    EXPECT_TRUE(g.hasEdge(2, 1));
    EXPECT_EQ(g.edgeCount(), 1);
}

TEST(GraphTest, AddEdgeToNonexistentVertex) {
    Graph g;
    g.addVertex(1);
    g.addEdge(1, 99);
    EXPECT_FALSE(g.hasEdge(1, 99));
    EXPECT_EQ(g.edgeCount(), 0);
}

TEST(GraphTest, RemoveEdge) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    g.removeEdge(1, 2);
    EXPECT_FALSE(g.hasEdge(1, 2));
    EXPECT_EQ(g.edgeCount(), 0);
}

TEST(GraphTest, GetNeighbors) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    auto neighbors = g.getNeighbors(1);
    EXPECT_EQ(neighbors.size(), 2);
    EXPECT_TRUE(std::find(neighbors.begin(), neighbors.end(), 2) != neighbors.end());
    EXPECT_TRUE(std::find(neighbors.begin(), neighbors.end(), 3) != neighbors.end());
}

TEST(GraphTest, ClearGraph) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    g.clear();
    EXPECT_EQ(g.vertexCount(), 0);
    EXPECT_EQ(g.edgeCount(), 0);
}

TEST(GraphTest, VertexCount) {
    Graph g;
    EXPECT_EQ(g.vertexCount(), 0);
    g.addVertex(1);
    EXPECT_EQ(g.vertexCount(), 1);
    g.addVertex(2);
    EXPECT_EQ(g.vertexCount(), 2);
    g.removeVertex(1);
    EXPECT_EQ(g.vertexCount(), 1);
}

TEST(GraphTest, EdgeCount) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    EXPECT_EQ(g.edgeCount(), 0);
    g.addEdge(1, 2);
    EXPECT_EQ(g.edgeCount(), 1);
    g.addEdge(2, 3);
    EXPECT_EQ(g.edgeCount(), 2);
    g.removeEdge(1, 2);
    EXPECT_EQ(g.edgeCount(), 1);
}