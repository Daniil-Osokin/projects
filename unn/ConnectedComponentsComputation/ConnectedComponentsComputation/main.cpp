#include <iostream>
#include <math.h>
#include <time.h>
#include "connected_components.hpp"

using namespace std;

int main(int argc, char* argv[])
{    
    const int n = 1000;
    const int m = n * n / 10; // log(float(n)) / log(2.0f);
    Graph g(n, m);

    const int repeats = 10;
    ConnectedComponents compNaive;
    ConnectedComponents compSet;
    float naiveTime = 0;
    float ramTime = 0;
    for (int i = 0; i < repeats; i++)
    {
        clock_t t1 = clock();
        compNaive.createWithNaiveAlgorithm(g);
        clock_t t2 = clock();
        naiveTime += float(t2 - t1) / CLOCKS_PER_SEC * 1000;

        t1 = clock();
        compSet.createWithSeparatedSetAlgorithm(g);
        t2 = clock();
        ramTime += float(t2 - t1) / CLOCKS_PER_SEC * 1000;       
    }
    cout << "Naive time: " << naiveTime / float(repeats) << " ms" << endl;
    cout << "Ram with set time: " << ramTime / float(repeats) << " ms" << endl;

    g.print();
    compNaive.print();
    compSet.print();

    return 0;
}
