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

static void thinningIterationOpt(Mat& im, Mat& im_next, int& removed, int iter)
{
    for (int i = 1; i < im.rows-1; i++)
    {
        uchar* row0 = im.row(i-1).data;
        uchar* row1 = im.row(i+0).data;
        uchar* row2 = im.row(i+1).data;
        uchar* mrow = im_next.row(i).data;
        for (int j = 1; j < im.cols-1; j++)
        {
            if (!row1[j]) continue;

            uchar p9 = row0[j-1];
            uchar p2 = row0[j];
            uchar p3 = row0[j+1];
            uchar p8 = row1[j-1];
            uchar p4 = row1[j+1];
            uchar p7 = row2[j-1];
            uchar p6 = row2[j];
            uchar p5 = row2[j+1];

            int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            if (B > 6 || B < 2) continue;
            int m1 = iter == 0 ? p2 * p4 * p6 : p2 * p4 * p8;
            if (m1 != 0) continue;
            int m2 = iter == 0 ? p4 * p6 * p8 : p2 * p6 * p8;
            if (m2 != 0) continue;

            int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) + 
                     (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) + 
                     (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                     (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

            if (A == 1)
            {
                mrow[j] = 0;
                removed++;
            }
        }
    }
}

void thinning_opt(Mat& im)
{
    im /= 255;
    Mat im_next;
    im.copyTo(im_next);

    int removed;
    do {
        removed = 0;
        thinningIterationOpt(im, im_next, removed, 0);
        im_next.copyTo(im);
        thinningIterationOpt(im, im_next, removed, 1);
        im_next.copyTo(im);
    } 
    while (removed > 0);

    im *= 255;
}

static void commonIteration(Mat& im, Mat& im_next, int& removed)
{

    for (int i = 1; i < 3; i++)
    {
        uchar* row0 = im.row(i-1).data;
        uchar* row1 = im.row(i+0).data;
        uchar* row2 = im.row(i+1).data;
        uchar* mrow = im_next.row(i).data;
        for (int j = 1; j < im.cols-1; j++)
        {
            if (!row1[j]) 
            {
                mrow[j] = 0;
                continue;
            }

            uchar p9 = row0[j-1];
            uchar p2 = row0[j];
            uchar p3 = row0[j+1];
            uchar p8 = row1[j-1];
            uchar p4 = row1[j+1];
            uchar p7 = row2[j-1];
            uchar p6 = row2[j];
            uchar p5 = row2[j+1];

            int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            if (B > 6 || B < 2) continue;
            int m1 = p2 * p4 * p6;
            if (m1 != 0) continue;
            int m2 = p4 * p6 * p8;
            if (m2 != 0) continue;

            int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                     (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                     (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                     (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

            if (A == 1)
            {
                mrow[j] = 0;
                removed++;
            }
        }
    }


    for (int i = 3; i < im.rows-1; i++)
    {
        uchar* row0 = im.row(i-1).data;
        uchar* row1 = im.row(i+0).data;
        uchar* row2 = im.row(i+1).data;
        uchar* mrow = im_next.row(i).data;

        uchar* nrow0 = im_next.row(i-1 - 2).data;
        uchar* nrow1 = im_next.row(i+0 - 2).data;
        uchar* nrow2 = im_next.row(i+1 - 2).data;
        uchar* nmrow = im.row(i - 2).data;

        for (int j = 1; j < im.cols-1; j++)
        {
            if (nrow1[j])
            {

                uchar p9 = nrow0[j-1];
                uchar p2 = nrow0[j];
                uchar p3 = nrow0[j+1];
                uchar p8 = nrow1[j-1];
                uchar p4 = nrow1[j+1];
                uchar p7 = nrow2[j-1];
                uchar p6 = nrow2[j];
                uchar p5 = nrow2[j+1];

                int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
                if ((2 <= B) && (B <= 6))
                {
                    int m1 = p2 * p4 * p8;
                    if (m1 == 0)
                    {
                        int m2 = p2 * p6 * p8;
                        if (m2 == 0)
                        {

                            int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                                (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                                (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                                (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

                            if (A == 1)
                            {
                                nmrow[j] = 0;
                                removed++;
                            }
                        }
                    }
                }
            }
            else
            {
                nmrow[j] = 0;
            }


            if (row1[j])
            {

                uchar p9 = row0[j-1];
                uchar p2 = row0[j];
                uchar p3 = row0[j+1];
                uchar p8 = row1[j-1];
                uchar p4 = row1[j+1];
                uchar p7 = row2[j-1];
                uchar p6 = row2[j];
                uchar p5 = row2[j+1];

                int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
                if ((2 <= B) && (B <= 6))
                {
                    int m1 = p2 * p4 * p6;
                    if (m1 == 0)
                    {
                        int m2 = p4 * p6 * p8;
                        if (m2 == 0)
                        {

                            int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                                (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                                (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                                (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

                            if (A == 1)
                            {
                                mrow[j] = 0;
                                removed++;
                            }
                        }
                    }
                }
            }
            else
            {
                mrow[j] = 0;
            }
        }
    }


    for (int i = im.rows-3; i < im.rows-1; i++)
    {
        uchar* row0 = im_next.row(i-1).data;
        uchar* row1 = im_next.row(i+0).data;
        uchar* row2 = im_next.row(i+1).data;
        uchar* mrow = im.row(i).data;
        for (int j = 1; j < im.cols-1; j++)
        {
            if (!row1[j]) 
            {
                mrow[j] = 0;
                continue;
            }

            uchar p9 = row0[j-1];
            uchar p2 = row0[j];
            uchar p3 = row0[j+1];
            uchar p8 = row1[j-1];
            uchar p4 = row1[j+1];
            uchar p7 = row2[j-1];
            uchar p6 = row2[j];
            uchar p5 = row2[j+1];

            int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            if (B > 6 || B < 2) continue;
            int m1 = p2 * p4 * p8;
            if (m1 != 0) continue;
            int m2 = p2 * p6 * p8;
            if (m2 != 0) continue;

            int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                     (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                     (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                     (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

            if (A == 1)
            {
                mrow[j] = 0;
                removed++;
            }
        }
    }
}

void thinning_opt_m(Mat& im)
{
    im /= 255;
    Mat im_next;
    im.copyTo(im_next);

    int removed;
    do {
        removed = 0;
        commonIteration(im, im_next, removed);
        //im.copyTo(im_next);
    } 
    while (removed > 0);

    im *= 255;
}
