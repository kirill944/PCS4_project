#include <iostream>
#include <fstream>
#include "Graph.h"

void saveGraphToFile(const Graph& g, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось создать файл!" << std::endl;
        return;
    }

    auto vertices = g.getVertices();
    file << vertices.size() << "\n";
    for (int v : vertices) file << v << " ";
    file << "\n";

    file << g.edgeCount() << "\n";
    for (int v : vertices) {
        auto neighbors = g.getNeighbors(v);
        for (int n : neighbors) {
            if (v < n) file << v << " " << n << "\n";
        }
    }
    file.close();
}

Graph loadGraphFromFile(const std::string& filename) {
    Graph g;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл!" << std::endl;
        return g;
    }

    int vertexCount, edgeCount;
    file >> vertexCount;

    for (int i = 0; i < vertexCount; i++) {
        int v;
        file >> v;
        g.addVertex(v);
    }

    file >> edgeCount;
    for (int i = 0; i < edgeCount; i++) {
        int from, to;
        file >> from >> to;
        g.addEdge(from, to);
    }

    return g;
}

int main() {
    std::cout << "=== Сценарий сохранения и загрузки графа ===" << std::endl;

    // Создаём тестовый граф
    Graph original;
    for (int i = 1; i <= 4; i++) original.addVertex(i);
    original.addEdge(1, 2);
    original.addEdge(2, 3);
    original.addEdge(3, 4);
    original.addEdge(4, 1);

    std::cout << "Исходный граф: 4 вершины, 4 ребра (цикл)" << std::endl;

    // Сохраняем
    saveGraphToFile(original, "test_graph.txt");
    std::cout << "Граф сохранён в test_graph.txt" << std::endl;

    // Загружаем
    Graph loaded = loadGraphFromFile("test_graph.txt");
    std::cout << "Граф загружен из test_graph.txt" << std::endl;

    // Проверяем
    if (loaded.vertexCount() == original.vertexCount() &&
        loaded.edgeCount() == original.edgeCount()) {
        std::cout << "Данные сохранены и загружены корректно!" << std::endl;
    } else {
        std::cout << "Ошибка: данные не совпадают!" << std::endl;
    }

    return 0;
}