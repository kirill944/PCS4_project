#include <gtest/gtest.h>
#include "GraphScene.h"
#include <QApplication>
#include <QGraphicsView>

// Необходим QApplication для работы с графическими объектами
class GraphSceneTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        // Создаём QApplication для тестов
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

TEST_F(GraphSceneTest, ClearEmptyScene) {
    // Очистка пустой сцены не должна вызывать ошибок
    EXPECT_NO_THROW(scene->clearScene());
}

TEST_F(GraphSceneTest, UpdateGraphWithEmptyGraph) {
    Graph g;
    std::unordered_map<int, QPointF> positions;

    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, {}));
}

TEST_F(GraphSceneTest, UpdateGraphWithSingleVertex) {
    Graph g;
    g.addVertex(1);

    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);

    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, {}));
}

TEST_F(GraphSceneTest, UpdateGraphWithMultipleVertices) {
    Graph g;
    for (int i = 1; i <= 5; i++) {
        g.addVertex(i);
    }
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);

    std::unordered_map<int, QPointF> positions;
    for (int i = 1; i <= 5; i++) {
        positions[i] = QPointF(i * 100, i * 100);
    }

    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, {}));
}

TEST_F(GraphSceneTest, HighlightVertex) {
    Graph g;
    g.addVertex(1);

    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);

    scene->updateGraph(g, positions, -1, {});

    // Подсветка существующей вершины
    EXPECT_NO_THROW(scene->highlightVertex(1, true));

    // Попытка подсветить несуществующую вершину
    EXPECT_NO_THROW(scene->highlightVertex(99, true));
}

TEST_F(GraphSceneTest, SetVertexDraggable) {
    // Проверяем, что включение/выключение перетаскивания не вызывает ошибок
    EXPECT_NO_THROW(scene->setVertexDraggable(true));
    EXPECT_NO_THROW(scene->setVertexDraggable(false));
}

TEST_F(GraphSceneTest, GetDraggedPositions) {
    Graph g;
    g.addVertex(1);

    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);

    scene->updateGraph(g, positions, -1, {});

    auto draggedPositions = scene->getDraggedPositions();
    // Сцена может вернуть позиции (даже если не было перемещений)
    EXPECT_TRUE(draggedPositions.empty() || draggedPositions.size() == 1);
}

TEST_F(GraphSceneTest, UpdateVertexPosition) {
    Graph g;
    g.addVertex(1);

    std::unordered_map<int, QPointF> positions;
    QPointF originalPos(100, 100);
    positions[1] = originalPos;

    scene->updateGraph(g, positions, -1, {});

    QPointF newPos(200, 200);
    EXPECT_NO_THROW(scene->updateVertexPosition(1, newPos));
}

TEST_F(GraphSceneTest, UpdateEdgePositions) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);

    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);
    positions[2] = QPointF(200, 200);

    scene->updateGraph(g, positions, -1, {});

    // Обновляем позиции рёбер
    std::unordered_map<int, QPointF> newPositions;
    newPositions[1] = QPointF(150, 150);
    newPositions[2] = QPointF(250, 250);

    EXPECT_NO_THROW(scene->updateEdgePositions(g, newPositions));
}

TEST_F(GraphSceneTest, UpdateGraphWithHighlightedVertex) {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);

    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(100, 100);
    positions[2] = QPointF(200, 200);

    // Обновляем с подсвеченной вершиной
    EXPECT_NO_THROW(scene->updateGraph(g, positions, 1, {}));
}

TEST_F(GraphSceneTest, UpdateGraphWithVisitedSet) {
    Graph g;
    for (int i = 1; i <= 5; i++) {
        g.addVertex(i);
    }

    std::unordered_map<int, QPointF> positions;
    for (int i = 1; i <= 5; i++) {
        positions[i] = QPointF(i * 100, 100);
    }

    std::unordered_set<int> visited = {1, 2, 3};

    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, visited));
}

TEST_F(GraphSceneTest, MultipleGraphUpdates) {
    Graph g1, g2;
    g1.addVertex(1);
    g2.addVertex(2);

    std::unordered_map<int, QPointF> positions1, positions2;
    positions1[1] = QPointF(100, 100);
    positions2[2] = QPointF(200, 200);

    // Последовательные обновления не должны вызывать ошибок
    EXPECT_NO_THROW(scene->updateGraph(g1, positions1, -1, {}));
    EXPECT_NO_THROW(scene->updateGraph(g2, positions2, -1, {}));
    EXPECT_NO_THROW(scene->updateGraph(g1, positions1, -1, {}));
}

TEST_F(GraphSceneTest, UpdateGraphWithComplexGraph) {
    Graph g;
    // Создаём граф-звезду
    for (int i = 1; i <= 10; i++) {
        g.addVertex(i);
        if (i > 1) {
            g.addEdge(1, i);
        }
    }

    std::unordered_map<int, QPointF> positions;
    // Располагаем вершины по кругу
    double radius = 200;
    double centerX = 400;
    double centerY = 300;

    for (int i = 1; i <= 10; i++) {
        double angle = 2 * M_PI * (i - 1) / 9;
        double x = centerX + radius * cos(angle);
        double y = centerY + radius * sin(angle);
        positions[i] = QPointF(x, y);
    }
    positions[1] = QPointF(centerX, centerY);

    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, {}));
}

TEST_F(GraphSceneTest, SceneBounds) {
    Graph g;
    g.addVertex(1);

    std::unordered_map<int, QPointF> positions;
    positions[1] = QPointF(-1000, -1000); // Далеко за пределами сцены

    EXPECT_NO_THROW(scene->updateGraph(g, positions, -1, {}));

    // Сцена должна корректно обрабатывать вершины за пределами
    auto draggedPositions = scene->getDraggedPositions();
    EXPECT_TRUE(draggedPositions.empty() || draggedPositions.size() == 1);
}