#include <gtest/gtest.h>
#include "Graph.h"

// ==================== Graph Tests ====================

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
    g.addEdge(1, 99); // 99 не существует
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

#include "AlgorithmRunner.h"

TEST(AlgorithmRunnerTest, BFSTraversalSimple) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    g.addEdge(1, 2);
    g.addEdge(1, 3);

    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);

    EXPECT_EQ(order.size(), 3);
    EXPECT_EQ(order[0], 1);
}

TEST(AlgorithmRunnerTest, BFSTraversalDisconnected) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    g.addEdge(1, 2);
    // вершина 3 изолирована

    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);

    EXPECT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 2);
}

TEST(AlgorithmRunnerTest, DFSTraversalSimple) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    AlgorithmRunner runner;
    auto order = runner.getDFSTraversal(g, 1);

    EXPECT_EQ(order.size(), 3);
    EXPECT_EQ(order[0], 1);
}

TEST(AlgorithmRunnerTest, EmptyGraphTraversal) {
    Graph g;
    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);
    EXPECT_TRUE(order.empty());
}