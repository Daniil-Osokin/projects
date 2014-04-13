#include <thinning_morph.hpp>

using namespace cv;

/**
 * Perform one thinning iteration.
 * Normally you wouldn't call this function directly from your code.
 *
 * @param  im    Binary image with range = 0-1
 * @param  iter  0=even, 1=odd
 */
static void thinningIteration(Mat& im, int iter)
{
    Mat marker = Mat::zeros(im.size(), CV_8UC1);

    for (int i = 1; i < im.rows-1; i++)
    {
        for (int j = 1; j < im.cols-1; j++)
        {
            uchar p2 = im.at<uchar>(i-1, j);
            uchar p3 = im.at<uchar>(i-1, j+1);
            uchar p4 = im.at<uchar>(i, j+1);
            uchar p5 = im.at<uchar>(i+1, j+1);
            uchar p6 = im.at<uchar>(i+1, j);
            uchar p7 = im.at<uchar>(i+1, j-1);
            uchar p8 = im.at<uchar>(i, j-1);
            uchar p9 = im.at<uchar>(i-1, j-1);

            int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) + 
                     (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) + 
                     (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                     (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
            int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
            int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                marker.at<uchar>(i,j) = 1;
        }
    }

    im &= ~marker;
}

void thinning_gold(Mat& im)
{
    im /= 255;

    Mat prev = Mat::zeros(im.size(), CV_8UC1);
    Mat diff;

    do {
        thinningIteration(im, 0);
        thinningIteration(im, 1);
        absdiff(im, prev, diff);
        im.copyTo(prev);
    } 
    while (countNonZero(diff) > 0);

    im *= 255;
}

static void make_lut(int lut[], const int it);
static void encode(const Mat& src, Mat& encoded);
static int mark_removed(const Mat& encoded, const int lut[], Mat& remove_buf);
static void update(const Mat& remove_buf, Mat& encoded, const int removed);

void thinning_opt(Mat& im)
{
    int lut1[256], lut2[256];
    make_lut(lut1, 0);
    make_lut(lut2, 1);
    Mat encoded;
    encode(im, encoded);

    Mat remove_buf(1, im.rows / 3 * im.cols, CV_32SC1);

    int total_removed = 0;
    do {
        total_removed = 0;
        int removed = mark_removed(encoded, lut1, remove_buf);
        total_removed += removed;
        update(remove_buf, encoded, removed);

        removed = mark_removed(encoded, lut2, remove_buf);
        total_removed += removed;
        update(remove_buf, encoded, removed);
    } 
    while (total_removed > 0);

    for (int y = 1; y < im.rows - 1; y++) {
        uchar* row = im.row(y).data;
        int* enc = (int*)encoded.row(y).data;
        for (int x = 1; x < im.cols - 1; x++) {
            row[x] = 255;
            if (enc[x] == 256)
                row[x] = 0;
        }
    }
}

static void update(const Mat& remove_buf, Mat& encoded, const int removed)
{
    uint* remove = (uint*)remove_buf.data;
    const size_t step = encoded.step1();
    for (int i = 0; i < removed; i++) {
        int* row = (int*)encoded.data + remove[i];
        row[-1-step] &= 239 + 256;
        row[+0-step] &= 247 + 256;
        row[+1-step] &= 251 + 256;

        row[-1     ] &= 223 + 256;
        row[+0     ]  = 256;
        row[+1     ] &= 253 + 256;

        row[-1+step] &= 191 + 256;
        row[+0+step] &= 127 + 256;
        row[+1+step] &= 254 + 256;
    }
}

static int mark_removed(const Mat& encoded, const int lut[], Mat& remove_buf)
{
    uint* remove = (uint*)remove_buf.data;
    uint step = encoded.step1();
    int removed = 0;
    for (int y = 1; y < encoded.rows - 1; y++) {
        uint* row = (uint*)encoded.row(y).data;
        for (int x = 1; x < encoded.cols - 1; x++) {
            if (row[x] < 256) {
                if (!lut[row[x]])
                    remove[removed++] = y*step + x;
            }
        }
    }
    return removed;
}

static void encode(const Mat& src, Mat& encoded) 
{
    encoded = Mat::zeros(src.size(), CV_32SC1);
    for (int i = 1; i < src.rows-1; i++)
    {
        uchar* row0 = src.row(i-1).data;
        uchar* row1 = src.row(i+0).data;
        uchar* row2 = src.row(i+1).data;
        int* enc = (int*)encoded.row(i).data;
        for (int j = 1; j < src.cols-1; j++)
        {
            if (!row1[j]) {
                enc[j] = 256;
                continue;
            }

            uchar p9 = row0[j-1] & 1;
            uchar p2 = row0[j] & 1;
            uchar p3 = row0[j+1] & 1;
            uchar p8 = row1[j-1] & 1;
            uchar p4 = row1[j+1] & 1;
            uchar p7 = row2[j-1] & 1;
            uchar p6 = row2[j] & 1;
            uchar p5 = row2[j+1] & 1;

            enc[j] = (p2 << 7) + (p3 << 6) + (p4 << 5) + (p5 << 4) + (p6 << 3) + (p7 << 2) + (p8 << 1) + p9;
            enc[j] += enc[j] == 0 ? 1 : 0;

        }
    }
}

static void make_lut(int lut[], const int it)
{
    for (int p2 = 0; p2 < 2; p2++) {
        for (int p3 = 0; p3 < 2; p3++) {
            for (int p4 = 0; p4 < 2; p4++) {
                for (int p5 = 0; p5 < 2; p5++) {
                    for (int p6 = 0; p6 < 2; p6++) {
                        for (int p7 = 0; p7 < 2; p7++) {
                            for (int p8 = 0; p8 < 2; p8++) {
                                for (int p9 = 0; p9 < 2; p9++) {
                                    int val = 1;
                                    int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) + 
                                        (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) + 
                                        (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                                        (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
                                    int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
                                    int m1 = it == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
                                    int m2 = it == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

                                    if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                                        val = 0;

                                    lut[(p2 << 7) + (p3 << 6) + (p4 << 5) + (p5 << 4) + (p6 << 3) + (p7 << 2) + (p8 << 1) + p9] = val;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
