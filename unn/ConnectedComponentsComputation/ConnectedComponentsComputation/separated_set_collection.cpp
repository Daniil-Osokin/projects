#include "separated_set_collection.hpp"

void SeparatedSetCollection::create(int x)
{
    nodes[x] = x;
    ranks[x] = 0;
}

SeparatedSetCollection::SeparatedSetCollection(int size)
{
    nodes = new int[size];
    ranks = new int[size];
    for (int i = 0; i < size; i++)
    {
        create(i);
    }
}

void SeparatedSetCollection::combine(int x, int y)
{
    if (ranks[x] < ranks[y])
    {
        nodes[x] = y;
    }
    else if (ranks[x] > ranks[y])
    {
        nodes[y] = x;
    }
    else
    {
        nodes[x] = y;
        ranks[y] += 1;
    }
}

int SeparatedSetCollection::find(int x) const
{
    while (nodes[x] != x)
    {
        x = nodes[x];
    }
    return x;
}

SeparatedSetCollection::~SeparatedSetCollection()
{
    delete[] nodes;
    delete[] ranks;
}
