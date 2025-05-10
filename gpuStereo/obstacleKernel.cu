#include <cuda_runtime.h>
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/cuda_stream_accessor.hpp>
#include <opencv2/core/cuda_types.hpp>
#include <opencv2/core.hpp>
#include "stereoObstacles.h"

// The actual CUDA kernel for obstacle detection
__global__ void obstacleKernel(const unsigned char* disparity,
                                      unsigned char* obstacles,
                                      double maxDistance,
                                      int rows, int cols)
{
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    int halfWindow = 6;
    double minY = 100.0;

    //left camera parameters
    double baseline = 60.0;
    double fx = 543;
    double ox = 317.872;
    double fy = 721.241;
    double oy = 228.513;
    unsigned char pixel;
    double disparityChange = 0.0;

    //printf("max distance: %f\n", maxDistance);
    pixel = disparity[row * cols + col];

    double z = (double) pixel;

    double distance;

    /*
    if (z > 0 && z < maxDistance) {
        distance = baseline*fx/z;
    } else {
        distance = maxDistance + 1; // Mark as out of range
    }
    */
    if(z > 0) distance = baseline * fx / z;
    else distance = maxDistance + 1;
    


    if(distance > 0.0 && distance < maxDistance){
        //printf("distance: %f\n", distance);
        obstacles[row * cols + col] = (unsigned char)(255);
    } else {
        obstacles[row * cols + col] = (unsigned char)(0);
    }

/*
    double disp = (double)disparity[row*cols + col];

    //compute z the distance from camera
    double z = baseline*fx/disp;
    double z1 = baseline*fx/disp1;
    double z2 = baseline*fx/disp2;

    //compute x the side to side distnace. Neg to left
    double x = z * (ox - (double)col) / fx;

    // compute y the distance above the ground. y pos up
    double y = cameraHeight + z*((double)row - oy)/fy;
    double y1 = cameraHeight + z1*((double)(row - 1) - oy)/fy;
    double y2 = cameraHeight + z2*((double)(row + 1) - oy)/fy;

    //check if obstacle is within detection zone
    if(z > maxZ || fabs(x) > maxX){
        obstacles[row*cols+col] = 0;
        return;
    } 

    //

    int obstacleCol = (int)(cols*z/maxDistance);
    int obstacleRow = (int)(rows*(maxX+x)/(2*maxX));
*/
    
}