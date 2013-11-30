#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
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

vector<int> parseFinishVertices(const char* finishVertices);
void addNewCycle(vector<Edge>& edges, vector<Edge>& cycle, int startEdgeId);
int getStartEdgeId(const vector<Edge>& edges, const vector<Edge>& cycle);
void init(map<string, int>& vertexMap, vector<Edge>& edges, int& vertexNumber);
Edge getUnbalancedEdge(const vector<Edge>& edges, const int vertexNumber);
}

int main(int argc, char* argv[])
{
    int vertexNumber = 0;
    map<string, int> vertexMap;
    vector<Edge> edges;
    init(vertexMap, edges, vertexNumber);
    const Edge unbalancedEdge = getUnbalancedEdge(edges, vertexNumber);
    edges.push_back(unbalancedEdge);

    int edgeStartId = 0;
    for (size_t i = 0; i < edges.size(); i++)
    {
        if (edges[i].v1 == unbalancedEdge.v2)
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
    vector<string> reconstructedString;
    for (size_t i = 0; i < cycle.size(); i++)
    {
        for (map<string, int>::const_iterator it = vertexMap.begin(); it != vertexMap.end(); it++)
        {
            if (it->second == cycle[i].v1)
            {
                reconstructedString.push_back(it->first);break;
            }
        }
    }
    for (size_t i = 0; i < reconstructedString.size() - 1; i++)
    {
        cout << reconstructedString[i][0];
    }
    cout << reconstructedString[reconstructedString.size() - 1] << endl;

    return 0;
}

namespace
{
Edge getUnbalancedEdge(const vector<Edge>& edges, const int vertexNumber)
{
    int unbalancedV1 = -1;
    int unbalancedV2 = -1;
    for (int i = 0; i < vertexNumber; i++)
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
    return Edge(unbalancedV1, unbalancedV2, 1);
}

void init(map<string, int>& vertexMap, vector<Edge>& edges, int& vertexNumber)
{
    FILE* file = fopen("input.txt", "r");
    if (!file)
    {
        cout << "Cannot open input file" << endl;
        return;
    }
    
    const int maxPatternSize = 100;
    char start[maxPatternSize];
    char finish[maxPatternSize];
    fscanf(file, "%s -> %s", start, finish);
    int size = 0;
    while (start[++size] != '\0');

    string v1(start, size);
    vertexNumber = 0;
    if (vertexMap.find(v1) == vertexMap.end())
    {
        vertexMap[v1] = vertexNumber;
        vertexNumber++;
    }
    
    string v2(finish, size);
    if (vertexMap.find(v2) == vertexMap.end())
    {
        vertexMap[v2] = vertexNumber;
        vertexNumber++;
    }
    edges.push_back(Edge(vertexMap[v1], vertexMap[v2], 1));

    while (fscanf(file, "%s -> %s", start, finish) != EOF)
    {
        v1 = string(start, size);
        if (vertexMap.find(v1) == vertexMap.end())
        {
            vertexMap[v1] = vertexNumber;
            vertexNumber++;
        }

        v2 = string(finish, size);
        if (vertexMap.find(v2) == vertexMap.end())
        {
            vertexMap[v2] = vertexNumber;
            vertexNumber++;
        }
        edges.push_back(Edge(vertexMap[v1], vertexMap[v2], 1));
    }
}

int getStartEdgeId(const vector<Edge>& edges, const vector<Edge>& cycle)
{
    for (size_t i = 0; i < cycle.size(); i++)
    {
        for (size_t j = 0; j < edges.size(); j++)
        {
            if (edges[j].label && cycle[i].v1 == edges[j].v1)
            {
                return (int)j;
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

    size_t startPos = 0;
    for (startPos = 0; startPos < cycle.size(); startPos++)
    {
        if (edges[startEdgeId].v1 == cycle[startPos].v1)
        {
            break;
        }
    }

    vector<Edge> commonCycle;
    for (size_t i = 0; i < startPos; i++)
    {
        commonCycle.push_back(cycle[i]);
    }
    for (size_t i = 0; i < newCycle.size(); i++)
    {
        commonCycle.push_back(newCycle[i]);
    }
    for (size_t i = startPos; i < cycle.size(); i++)
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
}