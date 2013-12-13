#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

using namespace std;

int main(int argc, char* argv[])
{
    map<char, int> blosumMap;
    blosumMap['A'] = 0;
    blosumMap['C'] = 1;
    blosumMap['D'] = 2;
    blosumMap['E'] = 3;
    blosumMap['F'] = 4;
    blosumMap['G'] = 5;
    blosumMap['H'] = 6;
    blosumMap['I'] = 7;
    blosumMap['K'] = 8;
    blosumMap['L'] = 9;
    blosumMap['M'] = 10;
    blosumMap['N'] = 11;
    blosumMap['P'] = 12;
    blosumMap['Q'] = 13;
    blosumMap['R'] = 14;
    blosumMap['S'] = 15;
    blosumMap['T'] = 16;
    blosumMap['V'] = 17;
    blosumMap['W'] = 18;
    blosumMap['Y'] = 19;

    const int blosumSize = 20;
    vector<vector<int> > blosum(blosumSize);
    for (int i = 0; i < blosumSize; i++)
    {
        blosum[i].resize(blosumSize, 0);
    }
    const string filename("blosum62.txt");
    ifstream file(filename.c_str());
    if (!file.is_open())
    {
        cout << "Couldn't open input file " << filename << endl;
    }
    for (int i = 0; i < blosumSize; i++)
    {
        string line;
        for (int j = 0; j < blosumSize - 1; j++)
        {
            getline(file, line, ' ');
            blosum[i][j] = atoi(line.c_str());
        }
        getline(file, line);
        blosum[i][blosumSize - 1] = atoi(line.c_str());
    }

    const string s1 = "PLEASANTLY";
    const string s2 = "MEANLY";
    const int sigma = 5;

    const int n = s1.length() + 1;
    vector<vector<int> > s(n);
    const int m = s2.length() + 1;
    for (int i = 0; i < s.size(); i++)
    {
        s[i].resize(m, 0);
    }
    for (int i = 0; i < n; i++)
    {
        s[i][0] = -sigma * i;
    }
    for (int j = 0; j < m; j++)
    {
        s[0][j] = -sigma * j;
    }

    vector<char> backlog;
    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < m; j++)
        {
            int diag = blosum[blosumMap[s1[i - 1]]][blosumMap[s2[j - 1]]];

            s[i][j] = max(diag + s[i - 1][j - 1], max(s[i - 1][j] - sigma, s[i][j - 1] - sigma));
        }
    }

    int i = n - 1;
    int j = m - 1;
    string alignS1 = "";
    string alignS2 = "";
    while (i > 0 || j > 0)
    {
        if (i > 0 && j > 0 && (s[i][j] == blosum[blosumMap[s1[i - 1]]][blosumMap[s2[j - 1]]] + s[i - 1][j - 1]))
        {
            alignS1 = s1[i - 1] + alignS1;
            alignS2 = s2[j - 1] + alignS2;
            i--;
            j--;
        }
        else if (i > 0 && (s[i][j] == s[i - 1][j] - sigma))
        {
            alignS1 = s1[i - 1] + alignS1;
            alignS2 = "-" + alignS2;
            i--;
        }
        else
        {
            alignS1 = "-" + alignS1;
            alignS2 = s2[j - 1] + alignS2;
            j--;
        }
    }

    cout << s[n - 1][m - 1] << endl;
    cout << alignS1 << endl;
    cout << alignS2 << endl;

    return 0;
}
