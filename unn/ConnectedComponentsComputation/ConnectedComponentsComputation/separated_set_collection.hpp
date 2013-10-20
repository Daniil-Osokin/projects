#ifndef __SEPARATED_SET_COLLECTION_HPP__
#define __SEPARATED_SET_COLLECTION_HPP__

class SeparatedSetCollection
{
public:
    SeparatedSetCollection(int size);
    ~SeparatedSetCollection();
    void combine(int x, int y);
    int find(int x) const;

private:
    void create(int x);
    int* nodes;
    int* ranks;
};

#endif
