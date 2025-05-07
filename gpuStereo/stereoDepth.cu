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
#include "stereoKernel.h"

using namespace cv;
using namespace std;

void stereoDepth(Mat* left, Mat* right, Mat* depth, double maxDistance, int rows, int cols) {

    int BLOCK_SIZE = 16; // use square blocks
    dim3 block(BLOCK_SIZE, BLOCK_SIZE);
    dim3 grid((cols + BLOCK_SIZE - 1) / BLOCK_SIZE, (rows + BLOCK_SIZE - 1) / BLOCK_SIZE);

    // Allocate device memory (GPU)
    uchar *d_left, *d_right, *d_depth;
    cudaMalloc((void**) &d_left, cols * rows * sizeof(uchar));
    cudaMalloc((void**) &d_right,cols * rows * sizeof(uchar));
    cudaMalloc((void**) &d_depth,cols * rows * sizeof(uchar));

    // Copy images from host (CPU RAM) to device (GPU)
    cudaMemcpy(d_left, left->data, cols * rows * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right,right->data,cols * rows * sizeof(uchar), cudaMemcpyHostToDevice);

    // Launch kernel
    stereoKernel<<<grid, block>>>(d_left, d_right, d_depth, maxDistance, rows, cols);
    cudaDeviceSynchronize();

    // Copy back the disparity map
    cudaMemcpy(depth->data, d_depth, cols * rows * sizeof(uchar), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_depth);
}
