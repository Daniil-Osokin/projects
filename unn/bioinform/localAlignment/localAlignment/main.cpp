#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

using namespace std;

int main(int argc, char* argv[])
{
    map<char, int> pamMap;
    pamMap['A'] = 0;
    pamMap['C'] = 1;
    pamMap['D'] = 2;
    pamMap['E'] = 3;
    pamMap['F'] = 4;
    pamMap['G'] = 5;
    pamMap['H'] = 6;
    pamMap['I'] = 7;
    pamMap['K'] = 8;
    pamMap['L'] = 9;
    pamMap['M'] = 10;
    pamMap['N'] = 11;
    pamMap['P'] = 12;
    pamMap['Q'] = 13;
    pamMap['R'] = 14;
    pamMap['S'] = 15;
    pamMap['T'] = 16;
    pamMap['V'] = 17;
    pamMap['W'] = 18;
    pamMap['Y'] = 19;

    const int pamSize = 20;
    vector<vector<int> > pam(pamSize);
    for (int i = 0; i < pamSize; i++)
    {
        pam[i].resize(pamSize, 0);
    }
    const string filename("pam250.txt");
    ifstream file(filename.c_str());
    if (!file.is_open())
    {
        cout << "Couldn't open input file " << filename << endl;
    }
    for (int i = 0; i < pamSize; i++)
    {
        string line;
        for (int j = 0; j < pamSize - 1; j++)
        {
            getline(file, line, ' ');
            pam[i][j] = atoi(line.c_str());
        }
        getline(file, line);
        pam[i][pamSize - 1] = atoi(line.c_str());
    }

    const string s1 = "MEANLY";
    const string s2 = "PENALTY";
    const int sigma = 5;

    const int n = s1.length() + 1;
    vector<vector<int> > s(n);
    const int m = s2.length() + 1;
    for (size_t i = 0; i < s.size(); i++)
    {
        s[i].resize(m, 0);
    }
    vector<vector<int> > ss(s);

    vector<char> backlog;
    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < m; j++)
        {
            int diag = pam[pamMap[s1[i - 1]]][pamMap[s2[j - 1]]];

            s[i][j] = max(0, max(diag + s[i - 1][j - 1], max(s[i - 1][j] - sigma, s[i][j - 1] - sigma)));
        }
    }

    int maxX = 0;
    int maxY = 0;
    int maxVal = s[maxX][maxY];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (s[maxX][maxY] < s[i][j])
            {
                maxX = i;
                maxY = j;
                maxVal = s[i][j];
            }
        }
    }

    int i = maxX;
    int j = maxY;
    string alignS1 = "";
    string alignS2 = "";
    while ((i > 0 || j > 0) && (s[i][j] != 0))
    {
        if (i > 0 && j > 0 && (s[i][j] == pam[pamMap[s1[i - 1]]][pamMap[s2[j - 1]]] + s[i - 1][j - 1]))
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
        else if (j > 0 && (s[i][j] == s[i][j - 1] - sigma))
        {
            alignS1 = "-" + alignS1;
            alignS2 = s2[j - 1] + alignS2;
            j--;
        }
    }

    cout << s[maxX][maxY] << endl;
    cout << alignS1 << endl;
    cout << alignS2 << endl;

    return 0;
}
