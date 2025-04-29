#include <cuda_runtime.h>
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/cuda_stream_accessor.hpp>
#include <opencv2/core/cuda_types.hpp>
#include <opencv2/core.hpp>
#include "stereoObstacles.h"

using namespace cv;
using namespace cv::cuda;

// The actual CUDA kernel for obstacle detection
__global__ void obstacleKernel(const unsigned char* disparity,
                                      unsigned char* obstacles,
                                      double maxDistance,
                                      int rows, int cols)
{
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    //left camera parameters
    double baseline = 60.0;
    double fx = 578.0;
    double ox;
    double fy;
    double oy;
    unsigned char pixel;
    double disparityChange = 0.0;

    pixel = disparity[rows * cols + col];

    double z = (double) pixel;

    double distance;
    if(z > 0) distance = (baseline * fx) / z;
    else distance = 0.0;

    if(distance > 0.0 && distance < maxDistance){
        obstacles[row * cols + col] = (unsigned char)(255);
    } else {
        obstacles[row * cols + col] = (unsigned char) 0;
    }
/*
        //obstacle map resolution mm
    double maxX = 2000;

    double disp = (double)disparity[row*col + col];

    //compute z the distance from camera
    double z = baseline*fx/disp;

    //compute x the side to side distnace. Neg to left
    double x = z * (col - ox) / fx;

    //compute obstacle location on the map
    int obstacleCol, obstacleRow;
    if(z>maxDistance || fabs(x)>maxX) return;

    obstacleCol = (int)(cols*z/maxDistance);
    obstacleRow = (int)(rows*(maxX+x)/(2*maxX));

*/


}