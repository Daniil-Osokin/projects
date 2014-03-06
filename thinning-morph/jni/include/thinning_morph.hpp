#ifndef THINNING_MORPH_HPP
#define THINNING_MORPH_HPP

/**
 * Code for thinning a binary image using Zhang-Suen algorithm.
 */
#include <opencv2/imgproc/imgproc.hpp>

/**
 * Function for thinning the given binary image
 *
 * @param  im  Binary image with range = 0-255
 */
void thinning_gold(cv::Mat& im);

/**
 * Function for thinning the given binary image
 * optimized version
 *
 * @param  im  Binary image with range = 0-255
 */
void thinning_opt(cv::Mat& im);

#endif // THINNING_MORPH_HPP
