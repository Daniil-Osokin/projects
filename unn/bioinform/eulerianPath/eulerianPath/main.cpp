#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>

using namespace std;

namespace
{
class Edge
{
public:
    Edge(int v1 = -1, int v2 = -1, int label = 0)
        : v1(v1),
          v2(v2),
          label(label) { }

    int v1;
    int v2;
    int label;
};
}

vector<int> parseFinishVertices(const char* finishVertices);
void addNewCycle(vector<Edge>& edges, vector<Edge>& cycle, int startEdgeId);
int getStartEdgeId(const vector<Edge>& edges, const vector<Edge>& cycle);

int main(int argc, char* argv[])
{
    FILE* file = fopen("input.txt", "r");
    if (!file)
    {
        cout << "Cannot open input file" << endl;
        return -1;
    }
    
    const int maxVertexNumber = 100;
    char start[maxVertexNumber];
    char finish[maxVertexNumber];
    vector<Edge> edges;
    int minV = 0;
    int maxV = -1;
    while (fscanf(file, "%s -> %s", start, finish) != EOF)
    {
        Edge e;
        e.v1 = atoi(start);
        vector<int> finishVertices = parseFinishVertices(finish);
        for (size_t i = 0; i < finishVertices.size(); i++)
        {
            e.v2 = finishVertices[i];
            e.label = 1;
            edges.push_back(e);
        }
        minV = min(minV, min(e.v1, e.v2));
        maxV = max(maxV, max(e.v1, e.v2));
    }

    int unbalancedV1 = -1;
    int unbalancedV2 = -1;
    for (size_t i = minV; i < maxV; i++)
    {
        int in = 0;
        int out = 0;
        for (size_t j = 0; j < edges.size(); j++)
        {
            if (edges[j].v1 == i)
            {
                out++;
            }
            if (edges[j].v2 == i)
            {
                in++;
            }            
        }
        if (in == out)
        {
            continue;
        }
        if (in > out)
        {
            unbalancedV1 = i;
        }
        else if (in < out)
        {
            unbalancedV2 = i;
        }
        if (unbalancedV1 != -1 && unbalancedV2 != -1)
        {
            break;
        }
    }
    edges.push_back(Edge(unbalancedV1, unbalancedV2, 1));

    int edgeStartId = 0;
    for (size_t i = 0; i < edges.size(); i++)
    {
        if (edges[i].v1 == unbalancedV2)
        {
            edgeStartId = i;
            break;
        }
    }

    vector<Edge> cycle;
    addNewCycle(edges, cycle, edgeStartId);
    int edgeId = getStartEdgeId(edges, cycle);
    while (edgeId != -1)
    {
        addNewCycle(edges, cycle, edgeId);
        edgeId = getStartEdgeId(edges, cycle);
    }
    cout << cycle[0].v1 << "->" << cycle[0].v2;
    for (size_t i = 1; i < cycle.size() - 1; i++)
    {
        cout << "->" << cycle[i].v2;
    }
    cout << endl;

    return 0;
}

int getStartEdgeId(const vector<Edge>& edges, const vector<Edge>& cycle)
{
    for (int i = 0; i < cycle.size(); i++)
    {
        for (int j = 0; j < edges.size(); j++)
        {
            if (edges[j].label && cycle[i].v1 == edges[j].v1)
            {
                return j;
            }
        }

    }
    return -1;
}

void addNewCycle(vector<Edge>& edges, vector<Edge>& cycle, int startEdgeId)
{
    edges[startEdgeId].label = 0;
    vector<Edge> newCycle;
    newCycle.push_back(edges[startEdgeId]);
    do
    {
        for (size_t i = 0; i < edges.size(); i++)
        {
            if (edges[i].label && edges[i].v1 == newCycle[newCycle.size() - 1].v2)
            {
                edges[i].label = 0;
                newCycle.push_back(edges[i]);
                break;
            }
        }
    } while (newCycle[0].v1 != newCycle[newCycle.size() - 1].v2);

    int startPos = 0;
    for (startPos = 0; startPos < cycle.size(); startPos++)
    {
        if (edges[startEdgeId].v1 == cycle[startPos].v1)
        {
            break;
        }
    }

    vector<Edge> commonCycle;
    for (int i = 0; i < startPos; i++)
    {
        commonCycle.push_back(cycle[i]);
    }
    for (int i = 0; i < newCycle.size(); i++)
    {
        commonCycle.push_back(newCycle[i]);
    }
    for (int i = startPos; i < cycle.size(); i++)
    {
        commonCycle.push_back(cycle[i]);
    }

    cycle = commonCycle;
}

vector<int> parseFinishVertices(const char* finishVertices)
{
    size_t start = 0;
    size_t finish = 0;
    vector<int> vertices;
    string vertex("");
    do
    {
        finish++;
        if (finishVertices[finish] == ',')
        {
            for (size_t i = start; i < finish; i++)
            {
                vertex += finishVertices[i];
            }
            vertices.push_back(atoi(vertex.c_str()));
            vertex = "";
            start = finish + 1;
        }

        
    } while (finishVertices[finish] != '\0');

    for (size_t i = start; i < finish; i++)
    {
        vertex += finishVertices[i];
    }
    vertices.push_back(atoi(vertex.c_str()));

    return vertices;
}