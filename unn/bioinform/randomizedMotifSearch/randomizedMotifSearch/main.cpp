#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <time.h>

using namespace std;

string getMostProbableKmer(const vector<string>& profile, const string& dnaRow, const size_t k);
int getScore(const vector<string>& motifs);

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

    vector<string> answer;
    for (size_t i = 0; i < dna.size(); i++)
    {
        answer.push_back(dna[i].substr(0, k));
    }

    srand(time(0));
    for (size_t ntimes = 0; ntimes < 1000; ntimes++)
    {
        vector<string> bestMotifs;
        for (vector<string>::const_iterator it = dna.begin(); it != dna.end(); it++)
        {
            const size_t pos = rand() % (dna[0].size() - k);
            bestMotifs.push_back((*it).substr(pos, k));
        }


        vector<string> motifs = bestMotifs;
        while (true)
        {
            const size_t t = dna.size();
            vector<string> tmpMotifs;
            for (size_t i = 0; i < t; i++)
            {
                tmpMotifs.push_back(getMostProbableKmer(motifs, dna[i], k));
            }
            motifs = tmpMotifs;
            if (getScore(motifs) < getScore(bestMotifs))
            {
                bestMotifs = motifs;
            }
            else
            {
                break;
            }
        }
        if (getScore(bestMotifs) < getScore(answer))
        {
            answer = bestMotifs;
        }
    }

    for (size_t i = 0; i < answer.size(); i++)
    {
        cout << answer[i] << endl;
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
        for (int j = 0; j < idSize; j++)
        {
            count[id[j]]++;
        }
        for (int j = 0; j < profileSize; j++)
        {
            count[profile[j][i]]++;
        }
        float probCount = 0.0;
        for (size_t j = 0; j < idSize; j++)
        {
            probCount += count[id[j]];
        }
        for (size_t j = 0; j < idSize; j++)
        {
            table[id[j]][i] = count[id[j]] / probCount;
        }
    }

    float maxProb = 0;
    string motif(dnaRow.substr(0, k));
    for (size_t pos = k; pos <= dnaRow.size(); pos++)
    {
        string kmer = dnaRow.substr(pos - k, k);
        float kmerProb = 1;
        for (int j = 0; j < k; j++)
        {
            kmerProb *= table[kmer[j]][j];
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