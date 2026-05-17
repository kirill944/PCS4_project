#include <gtest/gtest.h>
#include "AlgorithmRunner.h"

// ==================== Тесты для AlgorithmRunner ====================

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
    // BFS может посетить 2 или 3 вторым, проверяем наличие
    EXPECT_TRUE((order[1] == 2 && order[2] == 3) || (order[1] == 3 && order[2] == 2));
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

TEST(AlgorithmRunnerTest, BFSTraversalSingleVertex) {
    Graph g;
    g.addVertex(1);

    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);

    EXPECT_EQ(order.size(), 1);
    EXPECT_EQ(order[0], 1);
}

TEST(AlgorithmRunnerTest, BFSTraversalLinearChain) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    for (int i = 1; i <= 4; i++) g.addEdge(i, i + 1);

    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);

    EXPECT_EQ(order.size(), 5);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[4], 5);
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
    EXPECT_EQ(order[2], 3);
}

TEST(AlgorithmRunnerTest, DFSTraversalDisconnected) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    g.addEdge(1, 2);

    AlgorithmRunner runner;
    auto order = runner.getDFSTraversal(g, 1);

    EXPECT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 2);
}

TEST(AlgorithmRunnerTest, DFSTraversalSingleVertex) {
    Graph g;
    g.addVertex(1);

    AlgorithmRunner runner;
    auto order = runner.getDFSTraversal(g, 1);

    EXPECT_EQ(order.size(), 1);
    EXPECT_EQ(order[0], 1);
}

TEST(AlgorithmRunnerTest, EmptyGraphTraversal) {
    Graph g;
    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);
    EXPECT_TRUE(order.empty());
}

TEST(AlgorithmRunnerTest, StartVertexNotInGraph) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);

    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 99);
    EXPECT_TRUE(order.empty());
}