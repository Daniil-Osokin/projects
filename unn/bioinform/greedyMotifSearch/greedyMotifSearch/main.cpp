#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>

using namespace std;

string getMostProbableKmer(const vector<string>& profile, const string& dnaRow, const size_t k);
int getScore(const vector<string>& motifs);

int main(int argc, char* argv[])
{
    const size_t k = 3;
    ifstream file("input.txt");
    if (!file.is_open())
    {
        cout << "Cannot open input file" << endl;
    }
    
    string line;
    vector<string> dna;
    while (getline(file,line))
    {
        dna.push_back(line);
    }
    file.close();

    vector<string> bestMotifs;
    for (vector<string>::const_iterator it = dna.begin(); it != dna.end(); it++)
    {
        bestMotifs.push_back((*it).substr(0, k));
    }

    const size_t size = dna[0].size();
    for (size_t pos = k; pos <= size; pos++)
    {
        string motif(dna[0].substr(pos - k, k));
        const size_t t = dna.size();
        vector<string> profile;
        profile.push_back(motif);
        for (size_t i = 1; i < t; i++)
        {
            profile.push_back(getMostProbableKmer(profile, dna[i], k));
        }

        vector<string> motifs = profile;
        if (getScore(motifs) < getScore(bestMotifs))
        {
            bestMotifs = motifs;
        }
    }

    for (size_t i = 0; i < bestMotifs.size(); i++)
    {
        cout << bestMotifs[i] << endl;
    }
    

    return 0;
}

string getMostProbableKmer(const vector<string>& profile, const string& dnaRow, const size_t k)
{
    const char id[] = {'A', 'T', 'G', 'C'};
    const size_t idSize = 4;
    map<char, vector<float>> table;
    for (size_t i = 0; i < idSize; i++)
    {
        table[id[i]] = vector<float>(k);
    }

    for (int i = 0; i < k; i++)
    {
        size_t profileSize = profile.size();
        map<char, int> count;
        for (int j = 0; j < profileSize; j++)
        {
            count[profile[j][i]]++;
        }
        for (size_t j = 0; j < idSize; j++)
        {
            table[id[j]][i] = count[id[j]] / float(profileSize);
        }
    }

    float maxProb = 0;
    string motif("");
    for (size_t pos = k; pos <= dnaRow.size(); pos++)
    {
        string kmer = dnaRow.substr(pos - k, k);
        float kmerProb = 0;
        for (int j = 0; j < k; j++)
        {
            kmerProb += table[kmer[j]][j];
        }

        if (kmerProb > maxProb)
        {
            maxProb = kmerProb;
            motif = kmer;
        }

    }

    return motif;
}

int getScore(const vector<string>& motifs)
{
    const char id[] = {'A', 'T', 'G', 'C'};
    const size_t idSize = 4;
    map<char, vector<int>> count;
    for (size_t i = 0; i < idSize; i++)
    {
        count[id[i]] = vector<int>(motifs[0].size());
    }

    for (size_t i = 0; i < motifs.size(); i++)
    {
        for (size_t j = 0; j < motifs[0].size(); j++)
        {
            count[motifs[i][j]][j]++;
        }
    }

    int score = 0;
    for (int j = 0; j < motifs[0].size(); j++)
    {
        int sum = 0;
        int max = 0;
        for (size_t i = 0; i < idSize; i++)
        {
            int cur = count[id[i]][j];
            sum += cur;
            max = max < cur ? cur : max;
        }
        score += (sum - max);
    }

    return score;
}