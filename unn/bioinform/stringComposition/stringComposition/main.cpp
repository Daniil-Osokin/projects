#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[])
{
    string text = "TCGAGAAATC";
    const size_t k = 5;

    vector<string> kmers;
    for (size_t pos = k; pos <= text.size(); pos++)
    {
        kmers.push_back(text.substr(pos - k, k));
    }
    sort(kmers.begin(), kmers.end());

    for (size_t i = 0; i < kmers.size(); i++)
    {
        cout << kmers[i] << endl;
    }

    return 0;
}