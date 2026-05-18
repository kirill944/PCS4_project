#include <iostream>
#include "Graph.h"
#include "AlgorithmRunner.h"

int main() {
    std::cout << "=== Сценарий DFS (Поиск в глубину) ===" << std::endl;

    Graph g;
    for (int i = 1; i <= 5; i++) g.addVertex(i);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(2, 5);

    std::cout << "Граф: 1 - (2,3); 2 - (4,5)" << std::endl;

    AlgorithmRunner runner;
    auto order = runner.getDFSTraversal(g, 1);

    std::cout << "DFS обход с вершины 1: ";
    for (int v : order) std::cout << v << " ";
    std::cout << std::endl;

    std::cout << "Ожидаемый порядок: 1,2,4,5,3 или 1,3,2,5,4" << std::endl;
    std::cout << "Сценарий DFS выполнен успешно!" << std::endl;

    return 0;
}