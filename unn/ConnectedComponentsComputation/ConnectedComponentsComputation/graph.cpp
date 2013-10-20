#include <iostream>
#include "graph.hpp"

using namespace std;

Edge::Edge(int v1, int v2)
: v1(v1),
  v2(v2) { }

Graph::Graph()
: vertexNumber(0) { }

Graph::Graph(int vertexNumber, int edgeNumber)
{
    this->vertexNumber = vertexNumber;
    int v1 = 0; 
    int v2 = 0;
    for (int i = 0; i < edgeNumber; i++)
    {
        Edge e(v1, v2);
        addEdge(e);

        if (v2 == vertexNumber - 1)
        {
            v1++;
            v2 = v1;
        }
        else
        {        
            v2++;
        }
    }
}

const vector<Edge>& Graph::getEdges() const
{
    return edges;
}

void Graph::addEdge(const Edge& edge)
{
    edges.push_back(edge);
    vertexNumber = max(vertexNumber, max(edge.v1, edge.v2));
}

void Graph::print() const
{
    cout << "Vertex number: " << getVertexNumber() << endl;
    if (getVertexNumber() > 10) return;

    for(vector<Edge>::const_iterator e = edges.begin(); e != edges.end(); e++)
    {
        cout << e->v1 << " " << e->v2 << endl;
    }
}

int Graph::getVertexNumber() const
{
    return vertexNumber;
}
