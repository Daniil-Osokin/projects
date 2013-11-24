#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <time.h>

using namespace std;

string getProfileRandonlyGeneratedKmer(const vector<string>& profile, const string& dnaRow, const size_t k);
int getScore(const vector<string>& motifs);
vector<string> constructProfileWithoutMotif(const vector<string>& motifs, const size_t i);
bool compareOrderedProbs(const vector<float>& i, const vector<float>& j) { return (i[1] < j[1]); }

int main(int argc, char* argv[])
{
    const size_t k = 8;
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
    const size_t size = dna[0].size();
    srand(time(0));
    for (vector<string>::const_iterator it = dna.begin(); it != dna.end(); it++)
    {
        const size_t startPos = rand() % (size - k);
        bestMotifs.push_back((*it).substr(startPos, k));
    }

    vector<string> motifs = bestMotifs;
    const size_t n = 200000;
    const size_t t = dna.size();
    for (size_t j = 0; j < n; j++)
    {
        size_t i = rand() % t;
        vector<string> profile = constructProfileWithoutMotif(motifs, i);
        motifs[i] = getProfileRandonlyGeneratedKmer(profile, dna[i], k);
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

vector<string> constructProfileWithoutMotif(const vector<string>& motifs, const size_t i)
{
    vector<string> profile = motifs;
    profile.erase(profile.begin() + i);
    return profile;
}

string getProfileRandonlyGeneratedKmer(const vector<string>& profile, const string& dnaRow, const size_t k)
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
        for (int j = 0; j < idSize; j++)
        {
            count[id[j]]++;
        }
        for (int j = 0; j < profileSize; j++)
        {
            count[profile[j][i]]++;
        }
        float countColSum = 0.0f;
        for (size_t j = 0; j < idSize; j++)
        {
            countColSum += count[id[j]];
        }
        for (size_t j = 0; j < idSize; j++)
        {
            table[id[j]][i] = count[id[j]] / countColSum;
        }
    }

    vector<float> probs;
    float sum = 0.0f;
    for (size_t pos = k; pos <= dnaRow.size(); pos++)
    {
        string kmer = dnaRow.substr(pos - k, k);
        float kmerProb = 1;
        for (int j = 0; j < k; j++)
        {
            kmerProb *= table[kmer[j]][j];
        }
        sum += kmerProb;
        probs.push_back(kmerProb);
    }

    for (size_t i = 0; i < probs.size(); i++)
    {
        probs[i] /= sum;
    }
    vector<vector<float>> orderedProbs;
    for (size_t i = 0; i < probs.size(); i++)
    {
        vector<float> orderedProb;
        orderedProb.push_back(i);
        orderedProb.push_back(probs[i]);
        orderedProbs.push_back(orderedProb);
    }
    sort(orderedProbs.begin(), orderedProbs.end(), compareOrderedProbs);
    vector<float> distrib;
    distrib.push_back(orderedProbs[0][1]);
    distrib.push_back(orderedProbs[1][1]);
    for (size_t i = 2; i < probs.size(); i++)
    {
        distrib.push_back(distrib[i-1] + orderedProbs[i][1]);
    }
    const float probPos = float(rand() % 10000) / 10000.0f;
    string motif = dnaRow.substr(0, k);
    for (size_t i = 1; i < probs.size(); i++)
    {
        if (distrib[i-1] <= probPos && probPos < distrib[i])
        {
            motif = dnaRow.substr(orderedProbs[i][0], k);
            break;
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