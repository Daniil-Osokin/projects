#include <iostream>
#include "connected_components.hpp"

using namespace std;

void ConnectedComponents::print() const
{    
    if (connectedComponents.size() > 10) return;

    for (unsigned int i = 0; i < connectedComponents.size(); i++)
    {
        cout << "vertex: " << i << " label: " << connectedComponents[i] << endl;
    }
}

void ConnectedComponents::createWithNaiveAlgorithm(const Graph &graph)
{
    connectedComponents.clear();
    for (int i = 0; i < graph.getVertexNumber(); i++)
    {
        connectedComponents.push_back(i);
    }

    for (int i = 0; i < graph.getVertexNumber() - 1; i++)
    {
        for (vector<Edge>::const_iterator e = graph.getEdges().begin(); e != graph.getEdges().end(); e++)
        {
            int minLabel = min(connectedComponents[e->v1], connectedComponents[e->v2]);
            connectedComponents[e->v1] = minLabel;
            connectedComponents[e->v2] = minLabel;
        }
    }
}

void ConnectedComponents::createWithSeparatedSetAlgorithm(const Graph &graph)
{
    connectedComponents.clear();
    SeparatedSetCollection separatedSetCollection(graph.getVertexNumber());

    for (vector<Edge>::const_iterator e = graph.getEdges().begin(); e != graph.getEdges().end(); e++)
    {
        int parent1 = separatedSetCollection.find(e->v1);
        int parent2 = separatedSetCollection.find(e->v2);
        if (parent1 != parent2)
        {
            separatedSetCollection.combine(parent1, parent2);
        }
    }

    for (int i = 0; i < graph.getVertexNumber(); i++)
    {
        connectedComponents.push_back(separatedSetCollection.find(i));
    }
}
