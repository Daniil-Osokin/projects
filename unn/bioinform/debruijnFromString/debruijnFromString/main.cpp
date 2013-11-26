#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>

using namespace std;
using namespace tr1;

vector<tuple<string, vector<string> > > getDebruijnFromString(const string& text, const size_t k);

int main(int argc, char* argv[])
{
    string text = "AAGATTCTCTAC";
    const size_t k = 4;
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