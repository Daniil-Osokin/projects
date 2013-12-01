#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <fstream>

using namespace std;
using namespace tr1;

namespace
{
vector<tuple<string, vector<string> > > getDebruijnFromPatterns(const vector<string>& patterns);
map<string, tuple<int, int, int> > createVertexMap(const vector<string>& patterns);
void dfs(const string nextVertex, vector<string>& path, map<string, tuple<int, int, int> >& vertexMap, 
         const vector<tuple<string, vector<string> > >& debruijn);
}

int main(int argc, char* argv[])
{
    ifstream file("input.txt");
    if (!file.is_open())
    {
        cout << "Cannot open input file" << endl;
    }
    
    string line;
    vector<string> patterns;
    while (getline(file, line))
    {
        patterns.push_back(line);
    }
    file.close();
    vector<tuple<string, vector<string>> > debruijn = getDebruijnFromPatterns(patterns);
    map<string, tuple<int, int, int> > vertexMap = createVertexMap(patterns);
    for (size_t i = 0; i < debruijn.size(); i++)
    {
        string startVertex = get<0>(debruijn[i]);
        if ((get<0>(vertexMap[startVertex]) == 1 && get<1>(vertexMap[startVertex]) == 1) ||
            get<1>(vertexMap[startVertex]) == 0)
        {
            continue;
        }
        vector<string> finishVertices = get<1>(debruijn[i]);
        for (size_t j = 0; j < finishVertices.size(); j++)
        {
            vector<string> path;
            path.push_back(startVertex);
            dfs(finishVertices[j], path, vertexMap, debruijn);
        }
    }

    return 0;
}

namespace
{
map<string, tuple<int, int, int> > createVertexMap(const vector<string>& patterns)
{
    map<string, tuple<int, int, int> > vertexMap;
    const size_t k = patterns[0].size();
    for (size_t i = 0; i < patterns.size(); i++)
    {
        get<1>(vertexMap[patterns[i].substr(0, k - 1)])++;
        get<0>(vertexMap[patterns[i].substr(1, k - 1)])++;
    }

    return vertexMap;
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

void dfs(const string nextVertex, vector<string>& path, map<string, tuple<int, int, int> >& vertexMap, 
         const vector<tuple<string, vector<string> > >& debruijn)
{
    path.push_back(nextVertex);
    if (get<1>(vertexMap[nextVertex]) != 1 || get<0>(vertexMap[nextVertex]) != 1)
    {
        cout << path[0];
        for (size_t i = 1; i < path.size(); i++)
        {
            cout << path[i][path[0].size() - 1];
        }
        cout <<  endl;

    }
    else
    {
        for (size_t i = 0; i < debruijn.size(); i++)
        {
            if (get<0>(debruijn[i]) != nextVertex)
            {
                continue;
            }
            const vector<string>& finishVertices = get<1>(debruijn[i]);
            for (size_t j = 0; j < finishVertices.size(); j++)
            {
                dfs(finishVertices[j], path, vertexMap, debruijn);
            }
        }
    }
}
}