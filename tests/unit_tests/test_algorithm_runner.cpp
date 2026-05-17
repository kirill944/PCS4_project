#include <gtest/gtest.h>
#include "AlgorithmRunner.h"

// ==================== Тесты для AlgorithmRunner ====================
// Внимание: Тесты runBFS/runDFS пропущены, так как они требуют QEventLoop
// Для проверки асинхронной работы используйте отдельные сценарии

// 2.1 getBFSTraversal() - 7 тестов

TEST(AlgorithmRunnerTest, BFS_VertexCount1) {
    Graph g;
    g.addVertex(1);
    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 1);
    EXPECT_EQ(order[0], 1);
}

TEST(AlgorithmRunnerTest, BFS_VertexCount2) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 2);
}

TEST(AlgorithmRunnerTest, BFS_VertexCount3_Linear) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 3);
    EXPECT_EQ(order[0], 1);
}

TEST(AlgorithmRunnerTest, BFS_VertexCount3_Star) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 3);
    EXPECT_EQ(order[0], 1);
}

TEST(AlgorithmRunnerTest, BFS_VertexCount4_Square) {
    Graph g;
    for (int i = 1; i <= 4; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 1);
    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 4);
}

TEST(AlgorithmRunnerTest, BFS_VertexCount5_Path) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    for (int i = 1; i <= 4; i++) g.addEdge(i, i + 1);
    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 5);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[4], 5);
}

TEST(AlgorithmRunnerTest, BFS_VertexCount5_Disconnected) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(3, 4);
    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 2);
}

// 2.2 getDFSTraversal() - 7 тестов

TEST(AlgorithmRunnerTest, DFS_VertexCount1) {
    Graph g;
    g.addVertex(1);
    AlgorithmRunner runner;
    auto order = runner.getDFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 1);
    EXPECT_EQ(order[0], 1);
}

TEST(AlgorithmRunnerTest, DFS_VertexCount2) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    AlgorithmRunner runner;
    auto order = runner.getDFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], 1);
}

TEST(AlgorithmRunnerTest, DFS_VertexCount3_Linear) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    AlgorithmRunner runner;
    auto order = runner.getDFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 3);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[2], 3);
}

TEST(AlgorithmRunnerTest, DFS_VertexCount3_Star) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    AlgorithmRunner runner;
    auto order = runner.getDFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 3);
    EXPECT_EQ(order[0], 1);
}

TEST(AlgorithmRunnerTest, DFS_VertexCount4_Square) {
    Graph g;
    for (int i = 1; i <= 4; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 1);
    AlgorithmRunner runner;
    auto order = runner.getDFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 4);
}

TEST(AlgorithmRunnerTest, DFS_VertexCount5_Path) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    for (int i = 1; i <= 4; i++) g.addEdge(i, i + 1);
    AlgorithmRunner runner;
    auto order = runner.getDFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 5);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[4], 5);
}

TEST(AlgorithmRunnerTest, DFS_VertexCount5_Disconnected) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(3, 4);
    AlgorithmRunner runner;
    auto order = runner.getDFSTraversal(g, 1);
    EXPECT_EQ(order.size(), 2);
}

// 2.3 Обработка ошибок - 2 теста

TEST(AlgorithmRunnerTest, EmptyGraphTraversal) {
    Graph g;
    AlgorithmRunner runner;

    auto bfsOrder = runner.getBFSTraversal(g, 1);
    EXPECT_TRUE(bfsOrder.empty());

    auto dfsOrder = runner.getDFSTraversal(g, 1);
    EXPECT_TRUE(dfsOrder.empty());
}

TEST(AlgorithmRunnerTest, StartVertexNotInGraph) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);

    AlgorithmRunner runner;

    auto bfsOrder = runner.getBFSTraversal(g, 99);
    EXPECT_TRUE(bfsOrder.empty());

    auto dfsOrder = runner.getDFSTraversal(g, 99);
    EXPECT_TRUE(dfsOrder.empty());
}