#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>
#include <limits.h>
 
using namespace cv;
using namespace std;

void stereoKernel(unsigned char* left, unsigned char* right,
                              unsigned char* depth, double maxDistance, 
                              int rows, int cols);

