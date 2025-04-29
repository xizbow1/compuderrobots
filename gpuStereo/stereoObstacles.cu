#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include "obstacleKernel.h"

using namespace cv;
using namespace std;

void stereoObstacles(Mat* depth, Mat* obstacles, double maxDistance, int rows, int cols) {

    int BLOCK_SIZE = 16; // use square blocks
    dim3 block(BLOCK_SIZE, BLOCK_SIZE);
    dim3 grid((cols + BLOCK_SIZE - 1) / BLOCK_SIZE, (rows + BLOCK_SIZE - 1) / BLOCK_SIZE);

    // Allocate device memory (GPU)
    uchar *d_depth, *d_obstacles;
    cudaMalloc((void**) &d_obstacles, cols * rows * sizeof(uchar));
    cudaMalloc((void**) &d_depth, cols * rows * sizeof(uchar));

    // Copy images from host (CPU RAM) to device (GPU)
    cudaMemcpy(d_depth, depth->data, cols * rows * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_obstacles, obstacles->data,cols * rows * sizeof(uchar), cudaMemcpyHostToDevice);

    // Launch kernel
    obstacleKernel<<<grid, block>>>(d_depth, d_obstalces, maxDistance, rows, cols);
    cudaDeviceSynchronize();

    // Copy back the disparity map
    cudaMemcpy(obstacles->data, d_obstacles, cols * rows * sizeof(uchar), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_depth);
}
