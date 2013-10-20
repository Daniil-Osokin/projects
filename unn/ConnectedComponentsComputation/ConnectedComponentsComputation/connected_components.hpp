#ifndef __CONNECTED_COMPONENTS_HPP__
#define __CONNECTED_COMPONENTS_HPP__

#include <vector>
#include "graph.hpp"
#include "separated_set_collection.hpp"

class ConnectedComponents
{
public:
    void print() const;
    void createWithNaiveAlgorithm(const Graph& graph);
    void createWithSeparatedSetAlgorithm(const Graph& graph);

private:
    std::vector<int> connectedComponents;
};

#endif
