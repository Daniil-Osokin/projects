#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace
{
enum {DOWN = 0, RIGHT, DIAG};
vector<vector<int> > getBacktrack(const string& v, const string& w);
void outputLCS(const vector<vector<int> >& backtrack, const string& v, int i, int j);
}

int main(int argc, char* argv[])
{
    const string v = "AACCTTGG";
    const string w = "ACACTGTGA";
    vector<vector<int> > backtrack = getBacktrack(v, w);

    outputLCS(backtrack, v, (int)v.length(), (int)w.length());
    cout << endl;

    return 0;
}
namespace
{
vector<vector<int> > getBacktrack(const string& v, const string& w)
{
    const int n = (int)v.length() + 1;
    vector<vector<int> > s(n);
    const int m = (int)w.length() + 1;
    for (size_t i = 0; i < s.size(); i++)
    {
        s[i].resize(m);
    }
    vector<vector<int> > backtrack(s);

    s[0][0] = 0;
    for (int i = 1; i < n; i++)
    {
        s[i][0] = 0;
    }

    for (int j = 1; j < m; j++)
    {
        s[0][j] = 0;
    }

    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < m; j++)
        {
            int diagWeight = s[i - 1][j - 1];
            if (v[i - 1] == w[j - 1])
            {
                diagWeight++;
            }
            s[i][j] = max(diagWeight, max(s[i - 1][j], s[i][j - 1]));

            if (s[i][j] == s[i - 1][j])
            {
                backtrack[i][j] = DOWN;
            }
            else if (s[i][j] == s[i][j - 1])
            {
                backtrack[i][j] = RIGHT;
            }
            else
            {
                backtrack[i][j] = DIAG;
            }
        }
    }

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