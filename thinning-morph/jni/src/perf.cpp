#include <algorithm>
#include <limits>
#include <math.h>

#include <perf.hpp>

using namespace std;

#define DBL_EPSILON numeric_limits<double>::epsilon()

PerfMetrics calculate(vector<double>& times)
{
    sort(times.begin(), times.end());

    double gmean = 0;
    double gstddev = 0;
    int n = 0;
    for(vector<double>::const_iterator i = times.begin(); i != times.end(); ++i)
    {
        double x = *i;
        if (x < DBL_EPSILON) continue;
        double lx = log(x);

        ++n;
        double delta = lx - gmean;
        gmean += delta / n;
        gstddev += delta * (lx - gmean);
    }

    gstddev = n > 1 ? sqrt(gstddev / (n - 1)) : 0;

    vector<double>::const_iterator start = times.begin();
    vector<double>::const_iterator end = times.end();

    int offset = 0;
    if (gstddev > DBL_EPSILON)
    {
        double minout = exp(gmean - 3 * gstddev);
        double maxout = exp(gmean + 3 * gstddev);
        while(*start < minout) ++start, ++offset;
        do --end; while(*end > maxout);
        ++end;
    }

    n = 0;
    gmean = 0;
    gstddev = 0;
    double mean = 0;
    double stddev = 0;
    int m = 0;
    for(; start != end; ++start)
    {
        double x = *start;
        if (x > DBL_EPSILON)
        {
            double lx = log(x);
            ++m;
            double gdelta = lx - gmean;
            gmean += gdelta / m;
            gstddev += gdelta * (lx - gmean);
        }
        ++n;
        double delta = x - mean;
        mean += delta / n;
        stddev += delta * (x - mean);
    }

    PerfMetrics metrics;
    metrics.gmean = exp(gmean);
    metrics.gstddev = m > 1 ? sqrt(gstddev / (m - 1)) : 0;
    metrics.median = n % 2
            ? (double)times[offset + n / 2]
            : 0.5 * (times[offset + n / 2] + times[offset + n / 2 - 1]);

    return metrics;
}
