#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

using namespace std;

int main(int argc, char* argv[])
{
    ifstream file("input.txt");
    if (!file.is_open())
    {
        cout << "Cannot open input.txt" << endl;
    }

    string kmer;
    vector<string> kmers;
    while (getline(file, kmer))
    {
        kmers.push_back(kmer);
    }

    map<string, vector<string>> adjacencyList;
    size_t overlapSize = kmers[0].size() - 1;
    for (vector<string>::const_iterator suffix = kmers.begin(); suffix != kmers.end(); suffix++)
    {
        for (vector<string>::const_iterator prefix = kmers.begin(); prefix != kmers.end(); prefix++)
        {
            if ((*suffix).substr(1, overlapSize) == (*prefix).substr(0, overlapSize))
            {
                adjacencyList[(*suffix)].push_back((*prefix));
            }
        }
    }

    for (map<string, vector<string>>::const_iterator it = adjacencyList.begin(); it != adjacencyList.end(); it++)
    {
        for (size_t i = 0; i < it->second.size(); i++)
        {
            cout << it->first << " -> " << it->second[i] << endl;
        }
    }    

    return 0;
}