#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <fstream>

using namespace std;
using namespace tr1;

vector<tuple<string, vector<string> > > getDebruijnFromString(const string& text, const size_t k);
string getStringFrom(const vector<string>& patterns);

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

    string text = getStringFrom(patterns);
    const size_t k = patterns[0].size();
    vector<tuple<string, vector<string>> > debruijn = getDebruijnFromString(text, k);

    for (size_t i = 0; i < debruijn.size(); i++)
    {
        vector<string>& edges = get<1>(debruijn[i]);
        cout << get<0>(debruijn[i]) << " -> " << edges[0];
        for (size_t j = 1; j < edges.size(); j++)
        {
            cout << "," << edges[j];
        }
        cout << endl;

    }

    return 0;
}

string getStringFrom(const vector<string>& patterns)
{
    for (size_t id = 0; id < patterns.size(); id++)
    {
        string text = patterns[id];
        vector<string> bagOfPatterns = patterns;
        bagOfPatterns.erase(bagOfPatterns.begin() + id);
        for (size_t ntimes = 0; ntimes < patterns.size() - 1; ntimes++)
        {
            const size_t k = patterns[0].size();
            const string& prefix = text.substr(0, k - 1);
            const string& suffix = text.substr(text.size() - k + 1, k - 1);

            for (size_t i = 0; i < bagOfPatterns.size(); i++)
            {
                const string& patternPrefix = bagOfPatterns[i].substr(0, k - 1);
                const string& patternSuffix = bagOfPatterns[i].substr(1, k - 1);
                if (prefix == patternSuffix)
                {
                    text = bagOfPatterns[i][0] + text;
                    bagOfPatterns.erase(bagOfPatterns.begin() + i);
                    break;
                }
                if (suffix == patternPrefix)
                {
                    text += bagOfPatterns[i][k - 1];
                    bagOfPatterns.erase(bagOfPatterns.begin() + i);
                    break;
                }
            }
        }

        if (bagOfPatterns.size() == 0)
        {
            return text;
        }
    }
}

vector<tuple<string, vector<string> > > getDebruijnFromString(const string& text, const size_t k)
{
    vector<tuple<string, vector<string>> > debruijn;

    for (size_t pos = 0; pos <= text.size() - k; pos++)
    {
        const string kmer = text.substr(pos, k - 1);
        bool hasVertex = false;
        for (size_t i = 0; i < debruijn.size(); i++)
        {
            if (get<0>(debruijn[i]) == kmer)
            {
                hasVertex = true;
                break;
            }
        }
        if (hasVertex)
            continue;

        vector<string> edges;
        edges.push_back(text.substr(pos + 1, k - 1));
        for (size_t start = pos + 1; start < text.size() - (k - 1); start++)
        {
            if (kmer == text.substr(start, k - 1))
            {
                edges.push_back(text.substr(start + 1, k - 1));
            }
        }
        debruijn.push_back(make_tuple(kmer, edges));
    }

    return debruijn;
}