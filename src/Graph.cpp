#include "Graph.h"
#include <algorithm>

Graph::Graph() {}

Graph::~Graph() {}

void Graph::addVertex(int id) {
    vertices.insert(id);
    if (adjacencyList.find(id) == adjacencyList.end()) {
        adjacencyList[id] = std::unordered_set<int>();
    }
}

void Graph::removeVertex(int id) {
    vertices.erase(id);
    adjacencyList.erase(id);
    for (auto& pair : adjacencyList) {
        pair.second.erase(id);
    }
}

bool Graph::hasVertex(int id) const {
    return vertices.find(id) != vertices.end();
}

std::vector<int> Graph::getVertices() const {
    return std::vector<int>(vertices.begin(), vertices.end());
}

void Graph::addEdge(int from, int to) {
    if (hasVertex(from) && hasVertex(to)) {
        adjacencyList[from].insert(to);
        adjacencyList[to].insert(from);
    }
}

void Graph::removeEdge(int from, int to) {
    if (adjacencyList.find(from) != adjacencyList.end()) {
        adjacencyList[from].erase(to);
    }
    if (adjacencyList.find(to) != adjacencyList.end()) {
        adjacencyList[to].erase(from);
    }
}

bool Graph::hasEdge(int from, int to) const {
    if (adjacencyList.find(from) == adjacencyList.end()) return false;
    return adjacencyList.at(from).find(to) != adjacencyList.at(from).end();
}

std::vector<int> Graph::getNeighbors(int vertex) const {
    if (adjacencyList.find(vertex) == adjacencyList.end()) return {};
    return std::vector<int>(adjacencyList.at(vertex).begin(),
                            adjacencyList.at(vertex).end());
}

int Graph::vertexCount() const {
    return vertices.size();
}

int Graph::edgeCount() const {
    int count = 0;
    for (const auto& pair : adjacencyList) {
        count += pair.second.size();
    }
    return count / 2;
}

void Graph::clear() {
    vertices.clear();
    adjacencyList.clear();
}