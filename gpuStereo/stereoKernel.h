#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>
#include <limits.h>
 
 __global__ void stereoKernel(unsigned char* left, unsigned char* right,
                              unsigned char* depth, double maxDistance, 
                              int rows, int cols);

