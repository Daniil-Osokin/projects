#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <stdio.h>
#include <tuple>

using namespace std;
using namespace tr1;

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

void addNewCycle(vector<Edge>& edges, vector<Edge>& cycle, int startEdgeId);
int getStartEdgeId(const vector<Edge>& edges, const vector<Edge>& cycle);
void init(map<string, int>& vertexMap, vector<Edge>& edges, int& vertexNumber, vector<string>& rightReads);
Edge getUnbalancedEdge(const vector<Edge>& edges, const int vertexNumber);
}

int main(int argc, char* argv[])
{
    int vertexNumber = 0;
    map<string, int> vertexMap;
    vector<Edge> edges;
    vector<string> rightReads;
    init(vertexMap, edges, vertexNumber, rightReads);

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
    string reconstructedPart("");
    for (size_t i = 0; i < reconstructedString.size(); i++)
    {
        reconstructedPart += reconstructedString[i][0];
    }
    vector<string> unusedTails;
    const size_t readSize = rightReads[0].size();
    for (size_t i = 0; i < rightReads.size(); i++)
    {
        if (reconstructedPart.find(rightReads[i]) == string::npos)
        {
            unusedTails.push_back(rightReads[i]);
        }
    }

    while (!unusedTails.empty())
    {
        for (size_t suffixOffset = 1; suffixOffset < readSize; suffixOffset++)
        {
            bool isFound = false;
            string suffix = reconstructedPart.substr(reconstructedPart.size() - readSize + suffixOffset, readSize - suffixOffset);
            for (size_t i = 0; i < unusedTails.size(); i++)
            {
                if (suffix == unusedTails[i].substr(0, readSize - suffixOffset))
                {
                    reconstructedPart += unusedTails[i].substr(readSize - suffixOffset, suffixOffset);
                    unusedTails.erase(unusedTails.begin() + i);
                    isFound = true;
                    break;
                }
            }
            if (isFound)
            {
                break;
            }
        }
    }
    cout << reconstructedPart;

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

string getPrefix(const string& v1, const string& v2)
{
    return v1.substr(0, v1.size() - 1) + v2.substr(0, v2.size() - 1);
}

string getSuffix(const string& v1, const string& v2)
{
    return v1.substr(1, v1.size() - 1) + v2.substr(1, v2.size() - 1);
}

void init(map<string, int>& vertexMap, vector<Edge>& edges, int& vertexNumber, vector<string>& rightReads)
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
    fscanf(file, "%s | %s", start, finish);
    int size = 0;
    while (start[++size] != '\0');

    string v1(start, size);
    string v2(finish, size);
    vertexNumber = 0;
    string prefix = getPrefix(v1, v2);
    map<string, int> edgeMap;
    if (vertexMap.find(prefix) == vertexMap.end())
    {
        vertexMap[prefix] = vertexNumber++;
    }

    string suffix = getSuffix(v1, v2);    
    if (vertexMap.find(suffix) == vertexMap.end())
    {
        vertexMap[suffix] = vertexNumber++;
        rightReads.push_back(v2);
    }
    edges.push_back(Edge(vertexMap[prefix], vertexMap[suffix], 1));

    
    while (fscanf(file, "%s | %s", start, finish) != EOF)
    {
        v1 = string(start, size);
        v2 = string(finish, size);
        prefix = getPrefix(v1, v2);
        if (vertexMap.find(prefix) == vertexMap.end())
        {
            vertexMap[prefix] = vertexNumber++;
        }

        suffix = getSuffix(v1, v2);
        if (vertexMap.find(suffix) == vertexMap.end())
        {
            vertexMap[suffix] = vertexNumber++;            
            rightReads.push_back(v2);
        }
        edges.push_back(Edge(vertexMap[prefix], vertexMap[suffix], 1));
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
}