#ifndef OBSTACLE_KERNEL_H
#define OBSTACLE_Kernel_H

#include <opencv2/core.hpp>


void obstacleKernel(cv::Mat* depth, cv::Mat* obstacles, double maxDistance, int rows, int cols);

 #endif