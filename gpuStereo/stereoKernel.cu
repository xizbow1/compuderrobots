#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>
#include <limits.h>

__global__ void stereoKernel(unsigned char* left, unsigned char* right, 
                            unsigned char* disparity, double maxDistance,
                            int rows, int cols){

    
// compute the row and col of the pixel to be processed
int col = blockIdx.x*blockDim.x + threadIdx.x;
int row = blockIdx.y*blockDim.y + threadIdx.y;

    const int windowWidth = 5; //must be odd
    const int halfWindow = (windowWidth-1)/2;
    int disparityStep = 2;
    int windowStep = 2;
    double maxDisparity = 56.5;
    double contrast;
    double contrastThreshold = 5;
    
    unsigned char leftPixel;
    unsigned char rightPixel;
    unsigned char centerPixel;
    double disp = 0.0;
    double distance;
    double sumSqDiff;
    double minSumSqDiff = (double)INT_MAX*(double)INT_MAX;
    double diff;
    double intensity, minIntensity, maxIntensity;


    if(row < halfWindow || row > rows-halfWindow ||
        col < halfWindow || col > cols - halfWindow) return;

    // Compute the contrast for left window
    // if contrast too low return
    minIntensity = (double)(left[row*cols+col]);
    maxIntensity = minIntensity;

    // Compute the sums within the windowsin each image
    for(int i = -halfWindow; i < halfWindow + 1; i++){
        for(int j = -halfWindow; j < halfWindow + 1; j++){
            intensity = (double)(left[(row + i) * cols + (col + j)]);
            if(intensity < minIntensity) minIntensity = intensity;
            if(intensity > maxIntensity) maxIntensity = intensity;
        }
    }

    // Ignore any contrast below the threshold
    contrast = maxIntensity - minIntensity;
    if(contrast < contrastThreshold) return;

    // Compute sum of squred differences each shifted window
    for(int k=0; k < maxDisparity; k++){
        sumSqDiff=0.0;
        for(int i = -halfWindow; i < halfWindow+1;i++){
            for(int j = -halfWindow; j < halfWindow+1;j++){
                    if(row + i < rows && col + j < cols && 0 <= col + j - k && col + j - k < cols){
                        leftPixel = left[(row+i)*cols+(col+j)];
                        rightPixel = right[(row+i)*cols+(col+j-k)];
                        diff = (double)leftPixel-(double)rightPixel;
                        sumSqDiff += fabs(diff);
                    }
            }
        }

        //compute min sum square diff
        if(sumSqDiff < minSumSqDiff){
            minSumSqDiff = sumSqDiff;
            disp = (unsigned char)k;
        }
    }

    disparity[row*cols+col] = disp;

    /*
    // Replace SSD with NCC for better matching
for (int k = 0; k < maxDistance
; k += disparityStep) {
    double sumLeft = 0.0, sumRight = 0.0, sumLeftSq = 0.0, sumRightSq = 0.0, sumProduct = 0.0;
    int count = 0;

    for (int i = -halfWindow; i < halfWindow + 1; i += windowStep) {
        for (int j = -halfWindow; j < halfWindow + 1; j += windowStep) {
            if (row + i < rows && col + j < cols && 0 <= col + j - k && col + j - k < cols) {
                leftPixel = left[(row + i) * cols + (col + j)];
                rightPixel = right[(row + i) * cols + (col + j - k)];

                sumLeft += leftPixel;
                sumRight += rightPixel;
                sumLeftSq += leftPixel * leftPixel;
                sumRightSq += rightPixel * rightPixel;
                sumProduct += leftPixel * rightPixel;
                count++;
            }
        }
    }

    if (count > 0) {
        double meanLeft = sumLeft / count;
        double meanRight = sumRight / count;
        double numerator = sumProduct - count * meanLeft * meanRight;
        double denominator = sqrt((sumLeftSq - count * meanLeft * meanLeft) * 
                                  (sumRightSq - count * meanRight * meanRight));

        double ncc = (denominator > 0) ? numerator / denominator : 0.0;

        if (ncc > minSumSqDiff) { // Maximize NCC instead of minimizing SSD
            minSumSqDiff = ncc;
            disp = k;
        } 
    }
        disparity[row*cols+col] = (unsigned char) (disp);
}
    */

}
