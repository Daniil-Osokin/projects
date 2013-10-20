#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <vector>

struct Edge
{
    Edge(int v1, int v2);
    int v1;
    int v2;
};

class Graph
{
public:
    Graph();
    Graph(int vertexNumber, int edgeNumber);

    const std::vector<Edge>& getEdges() const;
    void addEdge(const Edge& edge);
    int getVertexNumber() const;

    void print() const;
private:
    std::vector<Edge> edges;
    int vertexNumber;

};

#endif
