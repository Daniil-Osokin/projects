#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <map>

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

vector<string> getBitStrings(unsigned int n);
vector<tuple<string, vector<string> > > getDebruijnFromPatterns(const vector<string>& patterns);
void init(const vector<tuple<string, vector<string>> >& debruijn,
          map<string, int>& vertexMap, vector<Edge>& edges, int& vertexNumber);
void addNewCycle(vector<Edge>& edges, vector<Edge>& cycle, int startEdgeId);
int getStartEdgeId(const vector<Edge>& edges, const vector<Edge>& cycle);
}

int main(int argc, char* argv[])
{
    const size_t k = 18;
    vector<string> patterns = getBitStrings(k);
    vector<tuple<string, vector<string>> > debruijn = getDebruijnFromPatterns(patterns);
    
    int vertexNumber = 0;
    map<string, int> vertexMap;
    vector<Edge> edges;
    init(debruijn, vertexMap, edges, vertexNumber);
    cout << "init" << endl;

    vector<Edge> cycle;
    addNewCycle(edges, cycle, 0);
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
    cout << reconstructedString[reconstructedString.size() - 1][0] << endl;

    return 0;
}

namespace
{
void init(const vector<tuple<string, vector<string>> >& debruijn,
          map<string, int>& vertexMap, vector<Edge>& edges, int& vertexNumber)
{
    for (vector<tuple<string, vector<string>> >::const_iterator it = debruijn.begin(); 
        it != debruijn.end(); it++)
    {
        string v1 = get<0>(*it);
        if (vertexMap.find(v1) == vertexMap.end())
        {
            vertexMap[v1] = vertexNumber;
            vertexNumber++;
        }

        vector<string> rightV = get<1>(*it);
        for (size_t i = 0; i < rightV.size(); i++)
        {
            string& v2 = rightV[i];
            if (vertexMap.find(v2) == vertexMap.end())
            {
                vertexMap[v2] = vertexNumber;
                vertexNumber++;
            }
            edges.push_back(Edge(vertexMap[v1], vertexMap[v2], 1));
        }
    }
}

vector<string> getBitStrings(unsigned int n)
{
    vector<string> result;

    if (n <= 1)
    {
        result.push_back("0");
        result.push_back("1");
    }
    else
    {
        vector<string> sub = getBitStrings(n - 1);
        for (vector<string>::const_iterator it = sub.begin(); it != sub.end(); ++it)
        {
            result.push_back(*it+'0');
            result.push_back(*it+'1');
        }
    }
    return result;
}

vector<tuple<string, vector<string> > > getDebruijnFromPatterns(const vector<string>& patterns)
{
    map<string, vector<string> > edges;
    const size_t k = patterns[0].size();
    for (size_t i = 0; i < patterns.size(); i++)
    {
        edges[patterns[i].substr(0, k - 1)].push_back(patterns[i].substr(1, k - 1));
    }
    
    vector<tuple<string, vector<string> > > debruijn;
    for (map<string, vector<string> >::const_iterator it = edges.begin(); it != edges.end(); it++)
    {
        debruijn.push_back(make_tuple(it->first, it->second));
    }    

    return debruijn;
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