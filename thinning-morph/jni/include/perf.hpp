#ifndef PERF_HPP
#define PERF_HPP

#include <vector>

struct PerfMetrics
{
    double gmean;
    double gstddev;
    double median;
};

PerfMetrics calculate(std::vector<double>& times);

#endif // PERF_HPP
