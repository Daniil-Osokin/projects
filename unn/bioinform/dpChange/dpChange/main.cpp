#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

static int dpChange(const int money, const vector<int>& coins);

int main(int argc, char* argv[])
{
    const string filename("input.txt");
    ifstream file(filename.c_str());
    if (!file.is_open())
    {
        cout << "Couldn't open " + filename << endl;
        return -1;
    }

    string line;
    getline(file, line);
    const int money = atoi(line.c_str());

    vector<int> coins;
    while (getline(file, line, ','))
    {
        coins.push_back(atoi(line.c_str()));
    }

    cout << dpChange(money, coins) << endl;
    
    return 0;
}

int dpChange(const int money, const vector<int>& coins)
{
    vector<int> minNumCoins;
    minNumCoins.push_back(0);
    for (int m = 1; m <= money; m++)
    {
        minNumCoins.push_back(numeric_limits<int>::max()/2);
        for (size_t i = 0; i < coins.size(); i++)
        {
            if (m >= coins[i])
            {
                if (minNumCoins[m - coins[i]] + 1 < minNumCoins[m])
                {
                    minNumCoins[m] = minNumCoins[m - coins[i]] + 1;
                }
            }
        }
    }

    return minNumCoins[minNumCoins.size() - 1];
}