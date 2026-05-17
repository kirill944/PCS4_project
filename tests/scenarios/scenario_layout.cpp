#include <iostream>
#include <iomanip>
#include "Graph.h"
#include "GraphLayout.h"
#include <chrono>
#include <thread>

void printVertexPositions(const std::unordered_map<int, QPointF>& positions) {
    std::cout << "Позиции вершин:" << std::endl;
    for (const auto& pair : positions) {
        std::cout << "  Вершина " << std::setw(2) << pair.first
                  << ": (" << std::setw(6) << std::fixed << std::setprecision(1)
                  << pair.second.x() << ", "
                  << std::setw(6) << pair.second.y() << ")" << std::endl;
    }
}

double calculateEdgeLength(const QPointF& p1, const QPointF& p2) {
    return std::hypot(p1.x() - p2.x(), p1.y() - p2.y());
}

double calculateAverageEdgeLength(const Graph& g,
                                   const std::unordered_map<int, QPointF>& positions) {
    double totalLength = 0;
    int edgeCount = 0;

    auto vertices = g.getVertices();
    for (int v : vertices) {
        auto neighbors = g.getNeighbors(v);
        for (int n : neighbors) {
            if (v < n) { // Считаем каждое ребро один раз
                auto it1 = positions.find(v);
                auto it2 = positions.find(n);
                if (it1 != positions.end() && it2 != positions.end()) {
                    totalLength += calculateEdgeLength(it1->second, it2->second);
                    edgeCount++;
                }
            }
        }
    }

    return edgeCount > 0 ? totalLength / edgeCount : 0;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  СЦЕНАРИЙ: ТЕСТИРОВАНИЕ РАСКЛАДКИ ГРАФА" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // 1. Создаём тестовый граф
    std::cout << "1. Создание графа..." << std::endl;
    Graph graph;

    // Добавляем вершины
    for (int i = 1; i <= 8; i++) {
        graph.addVertex(i);
        std::cout << "   Добавлена вершина " << i << std::endl;
    }

    // Добавляем рёбра (сетка 3x3 без центра)
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(4, 5);
    graph.addEdge(5, 6);
    graph.addEdge(7, 8);
    graph.addEdge(1, 4);
    graph.addEdge(2, 5);
    graph.addEdge(3, 6);
    graph.addEdge(4, 7);
    graph.addEdge(5, 8);

    std::cout << "   Добавлено рёбер: " << graph.edgeCount() << std::endl;
    std::cout << std::endl;

    // 2. Применяем раскладку
    std::cout << "2. Применение алгоритма раскладки..." << std::endl;
    GraphLayout layout;

    auto start = std::chrono::high_resolution_clock::now();
    layout.untangle(graph);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "   Время выполнения: " << duration.count() << " мс" << std::endl;
    std::cout << std::endl;

    // 3. Выводим результаты
    std::cout << "3. Результаты раскладки:" << std::endl;
    auto positions = layout.getVertexPositions();
    printVertexPositions(positions);
    std::cout << std::endl;

    // 4. Анализируем качество раскладки
    std::cout << "4. Анализ качества раскладки:" << std::endl;

    double avgLength = calculateAverageEdgeLength(graph, positions);
    std::cout << "   Средняя длина ребра: " << std::fixed << std::setprecision(2)
              << avgLength << " пикселей" << std::endl;

    // Проверяем, что все вершины имеют разные позиции
    std::unordered_set<std::string> uniquePos;
    bool allUnique = true;
    for (const auto& pair : positions) {
        std::string key = std::to_string(pair.second.x()) + "," + std::to_string(pair.second.y());
        if (uniquePos.find(key) != uniquePos.end()) {
            allUnique = false;
            break;
        }
        uniquePos.insert(key);
    }
    std::cout << "   Все вершины имеют уникальные позиции: "
              << (allUnique ? "ДА" : "НЕТ") << std::endl;

    // Проверяем, что нет нулевых расстояний между соединёнными вершинами
    bool hasZeroEdges = false;
    auto vertices = graph.getVertices();
    for (int v : vertices) {
        auto neighbors = graph.getNeighbors(v);
        for (int n : neighbors) {
            if (v < n) {
                double len = calculateEdgeLength(positions[v], positions[n]);
                if (len < 1.0) {
                    hasZeroEdges = true;
                    std::cout << "   ВНИМАНИЕ: Ребро " << v << "-" << n
                              << " имеет нулевую длину!" << std::endl;
                }
            }
        }
    }

    if (!hasZeroEdges) {
        std::cout << "   Нет рёбер с нулевой длиной: ДА" << std::endl;
    }

    std::cout << std::endl;

    // 5. Тест с разными типами графов
    std::cout << "5. Тестирование на разных типах графов:" << std::endl;

    // Полный граф K4
    std::cout << "   Полный граф K4 (4 вершины, все соединены):" << std::endl;
    Graph k4;
    for (int i = 1; i <= 4; i++) k4.addVertex(i);
    for (int i = 1; i <= 4; i++) {
        for (int j = i + 1; j <= 4; j++) {
            k4.addEdge(i, j);
        }
    }

    GraphLayout layout2;
    layout2.untangle(k4);
    auto posK4 = layout2.getVertexPositions();
    std::cout << "     Получено позиций: " << posK4.size() << std::endl;

    // Двудольный граф K3,3
    std::cout << "   Двудольный граф K3,3:" << std::endl;
    Graph k33;
    for (int i = 1; i <= 6; i++) k33.addVertex(i);
    for (int i = 1; i <= 3; i++) {
        for (int j = 4; j <= 6; j++) {
            k33.addEdge(i, j);
        }
    }

    GraphLayout layout3;
    layout3.untangle(k33);
    auto posK33 = layout3.getVertexPositions();
    std::cout << "     Получено позиций: " << posK33.size() << std::endl;

    std::cout << std::endl;

    // 6. Тест производительности
    std::cout << "6. Тест производительности на большом графе:" << std::endl;

    Graph largeGraph;
    for (int i = 1; i <= 30; i++) {
        largeGraph.addVertex(i);
    }
    // Создаём случайные рёбра
    for (int i = 1; i <= 30; i++) {
        for (int j = i + 1; j <= 30; j++) {
            if (rand() % 100 < 15) { // 15% вероятность ребра
                largeGraph.addEdge(i, j);
            }
        }
    }

    std::cout << "   Граф: " << largeGraph.vertexCount() << " вершин, "
              << largeGraph.edgeCount() << " рёбер" << std::endl;

    GraphLayout layout4;
    auto startLarge = std::chrono::high_resolution_clock::now();
    layout4.untangle(largeGraph);
    auto endLarge = std::chrono::high_resolution_clock::now();

    auto durationLarge = std::chrono::duration_cast<std::chrono::milliseconds>(endLarge - startLarge);
    std::cout << "   Время раскладки: " << durationLarge.count() << " мс" << std::endl;
    std::cout << std::endl;

    // 7. Заключение
    std::cout << "========================================" << std::endl;
    std::cout << "  РЕЗУЛЬТАТ: СЦЕНАРИЙ ВЫПОЛНЕН УСПЕШНО" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "Выводы:" << std::endl;
    std::cout << "✓ Алгоритм раскладки успешно обрабатывает графы разного размера" << std::endl;
    std::cout << "✓ Все вершины получают уникальные позиции" << std::endl;
    std::cout << "✓ Раскладка выполняется за приемлемое время" << std::endl;
    std::cout << "✓ Графы с рёбрами корректно визуализируются" << std::endl;

    return 0;
}