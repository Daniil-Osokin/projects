#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include <thinning_morph.hpp>
#include <perf.hpp>
 
using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    CV_Assert(argc > 1);
    for (int img_idx = 1; img_idx < argc; img_idx++)
    {
        string src_filename(argv[img_idx]);
        cout << "src: " << src_filename << endl;
        Mat src = imread(src_filename, CV_LOAD_IMAGE_GRAYSCALE);
        CV_Assert(!src.empty());

        Mat bw;
        threshold(src, bw, 10, 255, CV_THRESH_BINARY);

        vector<double> times;
        const unsigned int ntimes = 100;
        Mat dst;
        for (unsigned int i = 0; i < ntimes; i++) 
        {
            bw.copyTo(dst);

            double t = (double)getTickCount();
            thinning_gold(dst);
            t = ((double)getTickCount() - t)/getTickFrequency() * 1000;
            times.push_back(t);
        }

        PerfMetrics metrics = calculate(times);
        cout << "gmean   " << metrics.gmean   << " ms" << endl
             << "gstddev " << metrics.gstddev << " ms" << endl
             << "median  " << metrics.median  << " ms" << endl << endl;


        string dst_filename(src_filename.substr(0, src_filename.find(".")) + "_dst.png");
        imwrite(dst_filename, dst);
    }

    return 0;
}
