#ifndef STEREO_OBSTACLES_H
#define STEREO_OBSTACLES_H

#include <opencv2/core.hpp>

// Function declaration for the stereo obstacles detector
void stereoObstacles(cv::Mat* depth, cv::Mat* obstacles, double maxDistance, int rows, int cols);

#endif // STEREO_OBSTACLES_H