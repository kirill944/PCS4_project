#include <gtest/gtest.h>
#include "GraphScene.h"
#include <QApplication>
#include <QGraphicsView>

class GraphSceneTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        static int argc = 1;
        static char* argv[] = {const_cast<char*>("test")};
        app = new QApplication(argc, argv);
    }

    static void TearDownTestSuite() {
        delete app;
        app = nullptr;
    }

    void SetUp() override {
        scene = new GraphScene();
        view = new QGraphicsView(scene);
        view->show();
    }

    void TearDown() override {
        delete view;
        delete scene;
    }

    static QApplication* app;
    GraphScene* scene;
    QGraphicsView* view;
};

QApplication* GraphSceneTest::app = nullptr;

TEST_F(GraphSceneTest, UpdateGraph_1Vertex) {
    Graph g;
    g.addVertex(1);
    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);
    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, {}));
}

TEST_F(GraphSceneTest, UpdateGraph_2Vertices_WithEdge) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);
    positions[2] = QPointF(200, 200);
    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, {}));
}

TEST_F(GraphSceneTest, UpdateGraph_3Vertices_Triangle) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 1);
    std::unordered_map<int, QPointF> positions;
    for (int i = 1; i <= 3; i++) {
        positions[i] = QPointF(i * 100, 100);
    }
    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, {}));
}

TEST_F(GraphSceneTest, UpdateGraph_4Vertices_Square) {
    Graph g;
    for (int i = 1; i <= 4; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 1);
    std::unordered_map<int, QPointF> positions;
    for (int i = 1; i <= 4; i++) {
        positions[i] = QPointF(i * 80, 100);
    }
    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, {}));
}

TEST_F(GraphSceneTest, UpdateGraph_5Vertices_Star) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    for (int i = 2; i <= 5; i++) g.addEdge(1, i);
    std::unordered_map<int, QPointF> positions;
    for (int i = 1; i <= 5; i++) {
        positions[i] = QPointF(i * 70, 100);
    }
    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, {}));
}

TEST_F(GraphSceneTest, UpdateGraph_WithHighlightedVertex) {
    Graph g;
    g.addVertex(1);
    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);
    EXPECT_NO_THROW(scene->updateGraph(g, positions, 1, {}));
}

TEST_F(GraphSceneTest, UpdateGraph_WithVisitedSet) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    std::unordered_map<int, QPointF> positions;
    for (int i = 1; i <= 3; i++) positions[i] = QPointF(i * 100, 100);
    std::unordered_set<int> visited = {1, 2};
    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, visited));
}

TEST_F(GraphSceneTest, ClearScene_Empty) {
    EXPECT_NO_THROW(scene->clearScene());
}

TEST_F(GraphSceneTest, ClearScene_After1Vertex) {
    Graph g;
    g.addVertex(1);
    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);
    scene->updateGraph(g, positions, -1, {});
    EXPECT_NO_THROW(scene->clearScene());
}

TEST_F(GraphSceneTest, ClearScene_After3Vertices) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    std::unordered_map<int, QPointF> positions;
    for (int i = 1; i <= 3; i++) positions[i] = QPointF(i * 100, 100);
    scene->updateGraph(g, positions, -1, {});
    EXPECT_NO_THROW(scene->clearScene());
}

TEST_F(GraphSceneTest, ClearScene_After5Vertices) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    std::unordered_map<int, QPointF> positions;
    for (int i = 1; i <= 5; i++) positions[i] = QPointF(i * 100, 100);
    scene->updateGraph(g, positions, -1, {});
    EXPECT_NO_THROW(scene->clearScene());
}

TEST_F(GraphSceneTest, ClearScene_Twice) {
    Graph g;
    g.addVertex(1);
    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);
    scene->updateGraph(g, positions, -1, {});
    scene->clearScene();
    EXPECT_NO_THROW(scene->clearScene());  // Повторная очистка
}

TEST_F(GraphSceneTest, HighlightVertex_1Vertex_Toggle) {
    Graph g;
    g.addVertex(1);
    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);
    scene->updateGraph(g, positions, -1, {});
    EXPECT_NO_THROW(scene->highlightVertex(1, true));
    EXPECT_NO_THROW(scene->highlightVertex(1, false));
}

TEST_F(GraphSceneTest, HighlightVertex_3Vertices) {
    Graph g;
    for (int i = 1; i <= 3; i++) g.addVertex(i);
    std::unordered_map<int, QPointF> positions;
    for (int i = 1; i <= 3; i++) positions[i] = QPointF(i * 100, 100);
    scene->updateGraph(g, positions, -1, {});
    for (int i = 1; i <= 3; i++) {
        EXPECT_NO_THROW(scene->highlightVertex(i, true));
        EXPECT_NO_THROW(scene->highlightVertex(i, false));
    }
}

TEST_F(GraphSceneTest, HighlightVertex_NonExistent) {
    Graph g;
    g.addVertex(1);
    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);
    scene->updateGraph(g, positions, -1, {});
    EXPECT_NO_THROW(scene->highlightVertex(99, true));  // Не существует
}

TEST_F(GraphSceneTest, HighlightVertex_Sequential) {
    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    std::unordered_map<int, QPointF> positions;
    for (int i = 1; i <= 5; i++) positions[i] = QPointF(i * 100, 100);
    scene->updateGraph(g, positions, -1, {});
    for (int i = 1; i <= 5; i++) {
        scene->highlightVertex(i, true);
    }
    for (int i = 1; i <= 5; i++) {
        scene->highlightVertex(i, false);
    }
    EXPECT_NO_THROW();
}

TEST_F(GraphSceneTest, HighlightVertex_AllAtOnce) {
    Graph g;
    for (int i = 1; i <= 4; i++) g.addVertex(i);
    std::unordered_map<int, QPointF> positions;
    for (int i = 1; i <= 4; i++) positions[i] = QPointF(i * 100, 100);
    scene->updateGraph(g, positions, -1, {});
    for (int i = 1; i <= 4; i++) {
        scene->highlightVertex(i, true);
    }
    EXPECT_NO_THROW();
}