#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <opencv2/opencv.hpp>

#include <thinning_morph.hpp>
 
using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	CV_Assert(argc == 2);	
	string src_filename(argv[1]);
	Mat src = imread(src_filename, CV_LOAD_IMAGE_GRAYSCALE);
	CV_Assert(!src.empty());

    Mat bw;
    threshold(src, bw, 10, 255, CV_THRESH_BINARY);

    vector<double> times;
    const unsigned int ntimes = 10;
    Mat dst;
    for (unsigned int i = 0; i < ntimes; i++) 
    {
    	bw.copyTo(dst);

    	double t = (double)getTickCount();
    	thinning_gold(dst);
    	t = ((double)getTickCount() - t)/getTickFrequency() * 1000;
    	times.push_back(t);
	}
	sort(times.begin(), times.end());
	cout << "Me: "
	     << ((times.size() % 2) ? times[ntimes / 2] : 0.5 * (times[ntimes / 2 - 1] + times[ntimes / 2]))
	     << " ms" << endl;

    string dst_filename(src_filename.substr(0, src_filename.find(".")) + "_dst.png");
    imwrite(dst_filename, dst);

    return 0;
}
