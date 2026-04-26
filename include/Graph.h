#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

class Graph {
public:
    Graph();
    ~Graph();

    // Добавление/удаление вершин
    void addVertex(int id);
    void removeVertex(int id);
    bool hasVertex(int id) const;
    std::vector<int> getVertices() const;

    // Добавление/удаление рёбер
    void addEdge(int from, int to);
    void removeEdge(int from, int to);
    bool hasEdge(int from, int to) const;

    // Получение соседей вершины
    std::vector<int> getNeighbors(int vertex) const;

    // Количество вершин и рёбер
    int vertexCount() const;
    int edgeCount() const;

    // Очистка графа
    void clear();

private:
    std::unordered_map<int, std::unordered_set<int>> adjacencyList;
    std::unordered_set<int> vertices;
};

#endif // GRAPH_H