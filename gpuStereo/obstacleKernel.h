#ifndef OBSTACLE_KERNEL_H
#define OBSTACLE_Kernel_H

#include <opencv2/core.hpp>


__global__ void obstacleKernel(Mat* depth, Mat* obstacles, double maxDistance, int rows, int cols);

 #endif