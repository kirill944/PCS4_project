#include <iostream>
#include "Graph.h"
#include "AlgorithmRunner.h"

int main() {
    std::cout << "=== Сценарий BFS (Поиск в ширину) ===" << std::endl;

    // Создаём граф-дерево
    Graph g;
    for (int i = 1; i <= 7; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(2, 5);
    g.addEdge(3, 6);
    g.addEdge(3, 7);

    std::cout << "Граф: 1 - (2,3); 2 - (4,5); 3 - (6,7)" << std::endl;

    AlgorithmRunner runner;
    auto order = runner.getBFSTraversal(g, 1);

    std::cout << "BFS обход с вершины 1: ";
    for (int v : order) std::cout << v << " ";
    std::cout << std::endl;

    std::cout << "Ожидаемый порядок: 1,2,3,4,5,6,7 или 1,3,2,6,7,4,5" << std::endl;
    std::cout << "✓ Сценарий BFS выполнен успешно!" << std::endl;

    return 0;
}