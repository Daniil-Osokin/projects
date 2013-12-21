#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

namespace
{
enum {DOWN = 0, RIGHT, DIAG};
vector<int> getBacktrack(const vector<vector<int> >& weights);
void outputLCS(const vector<vector<int> >& backtrack, const string& v, int i, int j);
}

int main(int argc, char* argv[])
{
    const string filename = "input.txt";
    FILE* file = fopen(filename.c_str(), "r");
    if (!file)
    {
        cout << "Couldn't open input file " << filename << endl;
        return -1;
    }
    int source;
    fscanf(file, "%d\n", &source);
    int sink;
    fscanf(file, "%d\n", &sink);
    sink++;
    const int size = sink - source;
    vector<vector<int> > weights(size);
    for (int i = 0; i < size; i++)
    {
        weights[i].resize(size, -1);
    }

    int v1;
    int v2;
    int weight;
    while (fscanf(file, "%d->%d:%d\n", &v1, &v2, &weight) != EOF)
    {
        if (v1 < source || v2 > sink - 1)
        {
            continue;
        }
        weights[v1 - source][v2 - source] = weight;
    }
    fclose(file);

    vector<int> backtrack = getBacktrack(weights);
    backtrack.push_back(sink-1-source);
    vector<int> zippedBacktrack;
    zippedBacktrack.push_back(backtrack[0]);
    for (int i = 1; i < backtrack.size(); i++)
    {
        if (backtrack[i] > zippedBacktrack[zippedBacktrack.size() - 1])
        {
            zippedBacktrack.push_back(backtrack[i]);
        }
    }
    cout << zippedBacktrack[0] + source;
    for (int i = 1; i < zippedBacktrack.size(); i++)
    {
        cout << "->" << zippedBacktrack[i] + source;
    }
    cout << endl;

    return 0;
}
namespace
{
vector<int> getBacktrack(const vector<vector<int> >& weights)
{
    const int n = (int)weights.size();
    vector<int> s(n);
    vector<int> backtrack(s);

    for (int i = 1; i < n; i++)
    {
        int max = 0;
        for (int j = 0; j < n; j++)
        {
            if (weights[j][i] == -1)
            {
                continue;
            }
            if (max < s[j] + weights[j][i])
            {
                max = s[j] + weights[j][i];
                backtrack[i] = j;
            }
        }
        s[i] = max;
    }
    cout << s[n - 1] << endl;

    return backtrack;
}

void outputLCS(const vector<vector<int> >& backtrack, const string& v, int i, int j)
{
    if (i == 0 || j == 0)
    {
        return;
    }
    if (backtrack[i][j] == DOWN)
    {
        outputLCS(backtrack, v, i - 1, j);
    }
    else if (backtrack[i][j] == RIGHT)
    {
        outputLCS(backtrack, v, i, j - 1);
    }
    else
    {
        outputLCS(backtrack, v, i - 1, j - 1);
        cout << v[i - 1];
    }
}
}