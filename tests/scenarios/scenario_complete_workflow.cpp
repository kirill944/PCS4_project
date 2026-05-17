#include <iostream>
#include "Graph.h"
#include "AlgorithmRunner.h"

int main() {
    std::cout << "=== Полный сценарий работы с графом ===" << std::endl;

    // 1. Создание графа
    Graph g;
    std::cout << "1. Создаём граф..." << std::endl;
    for (int i = 1; i <= 6; i++) {
        g.addVertex(i);
        std::cout << "   Добавлена вершина " << i << std::endl;
    }

    // 2. Добавление рёбер
    std::cout << "2. Добавляем рёбра..." << std::endl;
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(2, 5);
    g.addEdge(3, 6);
    std::cout << "   Добавлено 5 рёбер" << std::endl;

    // 3. Вывод статистики
    std::cout << "3. Статистика графа:" << std::endl;
    std::cout << "   Вершин: " << g.vertexCount() << std::endl;
    std::cout << "   Рёбер: " << g.edgeCount() << std::endl;

    // 4. Запуск BFS
    std::cout << "4. Запускаем BFS с вершины 1:" << std::endl;
    AlgorithmRunner runner;
    auto bfs_order = runner.getBFSTraversal(g, 1);
    std::cout << "   Порядок BFS: ";
    for (int v : bfs_order) std::cout << v << " ";
    std::cout << std::endl;

    // 5. Запуск DFS
    std::cout << "5. Запускаем DFS с вершины 1:" << std::endl;
    auto dfs_order = runner.getDFSTraversal(g, 1);
    std::cout << "   Порядок DFS: ";
    for (int v : dfs_order) std::cout << v << " ";
    std::cout << std::endl;

    // 6. Удаление вершины
    std::cout << "6. Удаляем вершину 2..." << std::endl;
    g.removeVertex(2);
    std::cout << "   После удаления вершин: " << g.vertexCount() << std::endl;
    std::cout << "   После удаления рёбер: " << g.edgeCount() << std::endl;

    // 7. Очистка графа
    std::cout << "7. Очищаем граф..." << std::endl;
    g.clear();
    std::cout << "   Вершин после очистки: " << g.vertexCount() << std::endl;

    std::cout << "\n✓ Полный сценарий выполнен успешно!" << std::endl;

    return 0;
}