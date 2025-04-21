#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>
#include <limits.h>

__global__ void stereoKernel(unsigned char* left, unsigned char* right, unsigned char* depth, double maxDistance, int rows, int cols){

    
// compute the row and col of the pixel to be processed
int col = blockIdx.x*blockDim.x + threadIdx.x;
int row = blockIdx.y*blockDim.y + threadIdx.y;

// put your stereo matching code here
// This code should only be for one pixel
// See the video I posted on acceleration stereo on the GPU 


    const int windowWidth = 11; //must be odd
    const int halfWindow = (windowWidth-1)/2;
    double baseLine = 60.0;
    double focalLength = 578.0;
    double maxDisparity = 71;

    unsigned char leftPixel;
    unsigned char rightPixel;
    int disparity;
    double distance;
    double sumSqDiff;
    int minSumSqDiff = INT_MAX;
    int diff;

    if(row < halfWindow || row > rows-halfWindow || col < halfWindow || col > cols - halfWindow) return;

    //compute sum of squred differecnes each shifted window

    for(int k=0; k<maxDisparity;k++){
        sumSqDiff=0.0;
        for(int i = -halfWindow; i<halfWindow+1;i++){
            for(int j = -halfWindow; j<halfWindow+1;j++){

                    leftPixel = left[(row+i)*cols+(col+j)];
                    rightPixel = right[(row+i)*cols+(col+j-k)];
                    diff = leftPixel-rightPixel;
                    sumSqDiff += diff*diff;
            }
        }

        
        //compute min sum square diff
        if(sumSqDiff < minSumSqDiff){
            minSumSqDiff = sumSqDiff;
            disparity = k;
        }
    }

    //if we have a valid disparity, compute the distance and save it
    if(disparity > 0){
        distance = baseLine*focalLength/disparity;
        depth[row*cols+col] = (unsigned char) (255.0*distance/maxDistance);
    }else {
        depth[row*cols+col]=255;
    }
}
