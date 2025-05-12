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
int row1 = 124;
int col1 = 364;

    const int windowWidth = 13; //must be odd
    const int halfWindow = (windowWidth-1)/2;
    int disparityStep = 1
    int windowStep = 1;
    double minDisparity = 0.0;
    double maxDisparity = 64.0;
    double contrast;
    double contrastThreshold = 20;
    
    unsigned char leftPixel;
    unsigned char rightPixel;
    unsigned char centerPixel;
    double disp = 0.0;
    double distance;
    double sumSqDiff;
    double minSumSqDiff = (double)INT_MAX*(double)INT_MAX;
    double diff;
    double intensity, minIntensity, maxIntensity;


    if(row < halfWindow || row > rows-halfWindow || col < maxDisparity ||
        col < halfWindow || col > cols - halfWindow){
            disparity[row*cols+col] = 0;
            return;
        } 

    // Compute the contrast for left window
    // if contrast too low return
    minIntensity = (double)(left[row*cols+col]);
    maxIntensity = minIntensity;

    //printf("distance: %f\n", maxDistance);

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
    if(contrast < contrastThreshold){
        disparity[row*cols+col] = 0;
        return;
    }

    // Compute sum of squred differences each shifted window
    for(int k=0; k < maxDisparity; k++){
        sumSqDiff=0.0;
        for(int i = -halfWindow; i < halfWindow+1;i+=windowStep){
            for(int j = -halfWindow; j < halfWindow+1;j+=windowStep){
                    if(row + i < rows && col + j < cols && 0 <= col + j - k && col + j - k < cols){
                        leftPixel = left[(row+i)*cols+(col+j)];
                        rightPixel = right[(row+i)*cols+(col+j-k)];
                        diff = (double)leftPixel-(double)rightPixel;
                        sumSqDiff += fabs(diff);
                    }
            }
        }

        if(col == col1 && row == row1) printf("%f\n", sumSqDiff);
        //compute min sum square diff
        if(sumSqDiff < minSumSqDiff){
            minSumSqDiff = sumSqDiff;
            
            disp = (unsigned char)k;
        }

        
    }

    if(disp > 0){
        disparity[row * cols + col] = disp;
    }
    

    

/*
for (int k = 0; k < maxDisparity; k += disparityStep) {
    double sumLeft = 0.0, sumRight = 0.0, sumLeftSq = 0.0, sumRightSq = 0.0, sumProduct = 0.0;
    int count = 0;

    for (int i = -halfWindow; i <= halfWindow; i += windowStep) {
        for (int j = -halfWindow; j <= halfWindow; j += windowStep) {
            if (row + i >= 0 && row + i < rows &&
                col + j >= 0 && col + j < cols &&
                col + j - k >= 0 && col + j - k < cols) {
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

        double ncc = (denominator > 1e-6) ? numerator / denominator : 0.0;

        if (ncc > minSumSqDiff) { // Maximize NCC
            minSumSqDiff = ncc;
            disp = k;
        }
    }
}
disparity[row * cols + col] = (unsigned char)(disp);
*/
}
