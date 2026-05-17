#include <gtest/gtest.h>
#include "Graph.h"
#include <algorithm>

// ==================== Тесты для Graph ====================

// 1.1 addVertex() - 6 тестов

TEST(GraphTest, AddVertex_Single) {
    Graph g;
    g.addVertex(1);
    EXPECT_TRUE(g.hasVertex(1));
    EXPECT_EQ(g.vertexCount(), 1);
}

TEST(GraphTest, AddVertex_Multiple) {
    Graph g;
    for (int i = 1; i <= 5; i++) {
        g.addVertex(i);
    }
    EXPECT_EQ(g.vertexCount(), 5);
    for (int i = 1; i <= 5; i++) {
        EXPECT_TRUE(g.hasVertex(i));
    }
}

TEST(GraphTest, AddVertex_Duplicate) {
    Graph g;
    g.addVertex(1);
    g.addVertex(1);
    EXPECT_EQ(g.vertexCount(), 1);
}

TEST(GraphTest, AddVertex_NegativeId) {
    Graph g;
    g.addVertex(-5);
    EXPECT_TRUE(g.hasVertex(-5));
    EXPECT_EQ(g.vertexCount(), 1);
}

TEST(GraphTest, AddVertex_ZeroId) {
    Graph g;
    g.addVertex(0);
    EXPECT_TRUE(g.hasVertex(0));
    EXPECT_EQ(g.vertexCount(), 1);
}

TEST(GraphTest, AddVertex_LargeId) {
    Graph g;
    g.addVertex(10000);
    EXPECT_TRUE(g.hasVertex(10000));
    EXPECT_EQ(g.vertexCount(), 1);
}

// 1.2 removeVertex() - 6 тестов

TEST(GraphTest, RemoveVertex_Existing) {
    Graph g;
    g.addVertex(1);
    g.removeVertex(1);
    EXPECT_FALSE(g.hasVertex(1));
    EXPECT_EQ(g.vertexCount(), 0);
}

TEST(GraphTest, RemoveVertex_WithEdges) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    g.removeVertex(1);
    EXPECT_FALSE(g.hasVertex(1));
    EXPECT_FALSE(g.hasEdge(1, 2));
    EXPECT_TRUE(g.hasVertex(2));
    EXPECT_EQ(g.edgeCount(), 0);
}

TEST(GraphTest, RemoveVertex_NonExistent) {
    Graph g;
    g.addVertex(1);
    g.removeVertex(99);
    EXPECT_EQ(g.vertexCount(), 1);
}

TEST(GraphTest, RemoveVertex_FromEmpty) {
    Graph g;
    g.removeVertex(1);
    EXPECT_EQ(g.vertexCount(), 0);
}

TEST(GraphTest, RemoveVertex_Multiple) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    for (int i = 1; i <= 3; i++) g.removeVertex(i);
    EXPECT_EQ(g.vertexCount(), 2);
    EXPECT_FALSE(g.hasVertex(1));
    EXPECT_FALSE(g.hasVertex(2));
    EXPECT_FALSE(g.hasVertex(3));
    EXPECT_TRUE(g.hasVertex(4));
    EXPECT_TRUE(g.hasVertex(5));
}

TEST(GraphTest, RemoveVertex_AllVertices) {
    Graph g;
    for (int i = 1; i <= 4; i++) g.addVertex(i);
    for (int i = 1; i <= 4; i++) g.removeVertex(i);
    EXPECT_EQ(g.vertexCount(), 0);
}

// 1.3 addEdge() - 6 тестов (без петли)

TEST(GraphTest, AddEdge_Simple) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    EXPECT_TRUE(g.hasEdge(1, 2));
    EXPECT_TRUE(g.hasEdge(2, 1));
    EXPECT_EQ(g.edgeCount(), 1);
}

TEST(GraphTest, AddEdge_Multiple) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(1, 3);
    EXPECT_EQ(g.edgeCount(), 3);
}

TEST(GraphTest, AddEdge_Duplicate) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    g.addEdge(1, 2);
    EXPECT_EQ(g.edgeCount(), 1);
}

TEST(GraphTest, AddEdge_ToNonexistentVertex) {
    Graph g;
    g.addVertex(1);
    g.addEdge(1, 99);
    EXPECT_FALSE(g.hasEdge(1, 99));
    EXPECT_EQ(g.edgeCount(), 0);
}

TEST(GraphTest, AddEdge_FromNonexistentVertex) {
    Graph g;
    g.addVertex(1);
    g.addEdge(99, 1);
    EXPECT_FALSE(g.hasEdge(99, 1));
    EXPECT_EQ(g.edgeCount(), 0);
}

TEST(GraphTest, AddEdge_BothNonexistent) {
    Graph g;
    g.addEdge(99, 100);
    EXPECT_EQ(g.edgeCount(), 0);
}

// 1.4 removeEdge() - 6 тестов

TEST(GraphTest, RemoveEdge_Existing) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    g.removeEdge(1, 2);
    EXPECT_FALSE(g.hasEdge(1, 2));
    EXPECT_EQ(g.edgeCount(), 0);
}

TEST(GraphTest, RemoveEdge_NonExistent) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.removeEdge(1, 2);
    EXPECT_EQ(g.edgeCount(), 0);
}

TEST(GraphTest, RemoveEdge_OneDirection) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    g.removeEdge(2, 1);
    EXPECT_FALSE(g.hasEdge(1, 2));
}

TEST(GraphTest, RemoveEdge_Multiple) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(1, 3);
    g.removeEdge(1, 2);
    g.removeEdge(2, 3);
    EXPECT_EQ(g.edgeCount(), 1);
    EXPECT_TRUE(g.hasEdge(1, 3));
}

TEST(GraphTest, RemoveEdge_AfterVertexRemoval) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    g.removeVertex(1);
    g.removeEdge(1, 2);
    EXPECT_EQ(g.edgeCount(), 0);
}

TEST(GraphTest, RemoveEdge_FromEmptyGraph) {
    Graph g;
    g.removeEdge(1, 2);
    EXPECT_EQ(g.edgeCount(), 0);
}

// 1.5 getNeighbors() - 5 тестов

TEST(GraphTest, GetNeighbors_Simple) {
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

TEST(GraphTest, GetNeighbors_Empty) {
    Graph g;
    g.addVertex(1);
    auto neighbors = g.getNeighbors(1);
    EXPECT_TRUE(neighbors.empty());
}

TEST(GraphTest, GetNeighbors_NonExistentVertex) {
    Graph g;
    auto neighbors = g.getNeighbors(99);
    EXPECT_TRUE(neighbors.empty());
}

TEST(GraphTest, GetNeighbors_FullGraph) {
    Graph g;
    for (int i = 1; i <= 4; i++) g.addVertex(i);
    for (int i = 1; i <= 4; i++) {
        for (int j = i + 1; j <= 4; j++) {
            g.addEdge(i, j);
        }
    }
    auto neighbors = g.getNeighbors(1);
    EXPECT_EQ(neighbors.size(), 3);
}

TEST(GraphTest, GetNeighbors_OneNeighbor) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    auto neighbors = g.getNeighbors(1);
    EXPECT_EQ(neighbors.size(), 1);
    EXPECT_EQ(neighbors[0], 2);
}

// 1.6 clear() - 5 тестов

TEST(GraphTest, Clear_EmptyGraph) {
    Graph g;
    g.clear();
    EXPECT_EQ(g.vertexCount(), 0);
    EXPECT_EQ(g.edgeCount(), 0);
}

TEST(GraphTest, Clear_WithVerticesOnly) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    g.clear();
    EXPECT_EQ(g.vertexCount(), 0);
}

TEST(GraphTest, Clear_WithEdges) {
    Graph g;
    for (int i = 1; i <= 4; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.clear();
    EXPECT_EQ(g.vertexCount(), 0);
    EXPECT_EQ(g.edgeCount(), 0);
}

TEST(GraphTest, Clear_Twice) {
    Graph g;
    g.addVertex(1);
    g.clear();
    g.clear();
    EXPECT_EQ(g.vertexCount(), 0);
}

TEST(GraphTest, Clear_AfterOperations) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.clear();
    g.addVertex(10);
    EXPECT_EQ(g.vertexCount(), 1);
    EXPECT_EQ(g.edgeCount(), 0);
}