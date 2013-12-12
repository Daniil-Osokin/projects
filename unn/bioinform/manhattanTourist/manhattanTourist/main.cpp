#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

static int manhsttanTourist(int n, int m, const vector<vector<int> >& down, const vector<vector<int> >& right);

int main(int argc, char* argv[])
{
    const string filename("input.txt");
    ifstream file(filename.c_str());
    if (!file.is_open())
    {
        cout << "Couldn't open input file " << filename << endl;
    }

    string line;
    getline(file, line);
    int n = atoi(line.c_str()) + 1;

    getline(file, line);
    int m = atoi(line.c_str()) + 1;

    vector<vector<int> > down(n - 1);
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < m - 1; j++)
        {
            getline(file, line, ' ');
            down[i].push_back(atoi(line.c_str()));
        }
        getline(file, line);
        down[i].push_back(atoi(line.c_str()));
    }
    getline(file, line); // skip `-`

    vector<vector<int> > right(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m - 2; j++)
        {
            getline(file, line, ' ');
            right[i].push_back(atoi(line.c_str()));
        }
        getline(file, line);
        right[i].push_back(atoi(line.c_str()));
    }

    cout << manhsttanTourist(n, m, down, right) << endl;

    return 0;
}

int manhsttanTourist(int n, int m, const vector<vector<int> >& down, const vector<vector<int> >& right)
{
    vector<vector<int> > s(right.size());
    for (size_t i = 0; i < s.size(); i++)
    {
        s[i].resize(down[0].size());
    }

    s[0][0] = 0;
    for (int i = 1; i < n; i++)
    {
        s[i][0] = s[i - 1][0] + down[i - 1][0];
    }

    for (int j = 1; j < m; j++)
    {
        s[0][j] = s[0][j - 1] + right[0][j - 1];
    }

    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < m; j++)
        {
            s[i][j] = max(s[i - 1][j] + down[i - 1][j], s[i][j - 1] + right[i][j - 1]);
        }
    }

    return s[s.size() - 1][s[0].size() - 1];
}