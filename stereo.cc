#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <string>
#include <sstream>
#include <omp.h>
#include "utils.h"
#include "imageUtils.h"


using namespace std;

double cocaCola(float* x, int rowX, int colX, float* y, int rowY, int colY, int n) {
    double prod = 0.0;
    double sqX = 0.0;
    double sqY = 0.0;
    double mX = 0.0;
    double mY = 0.0;
    double cov = 0.0;
    double coeff = 0.0;
    double stdDevX = 0.0;
    double stdDevY = 0.0;

    for(int row = 0; row < rowX; row++){
        for(int col = 0; col < colX; col++){
        //find the squares of both x and y
        sqX += x[row * colX + col] * x[row * colX + col];
        sqY += y[row * colY + col] * y[row * colY + col];

        //find the sum of x and y
        mX += x[row * colX + col];
        mY += y[row * colY + col];

        //find the product of x and y
        prod += x[row * colX + col] * y[row * colY + col];
        }
    }

    // find the mean of x and y squared
    sqX = sqX / n;
    sqY = sqY / n;

    // find the mean of both mX and mY
    mX = mX / n;
    mY = mY / n;

    // find the convergence 
    cov = (prod / n) - (mX * mY);

    // find the standard deviation
    stdDevX = sqrt(sqX - (mX * mX));
    stdDevY = sqrt(sqY - (mY * mY));

    // Check for zero standard deviation to avoid division by zero
    if (stdDevX == 0 || stdDevY == 0) {
        coeff = 0.0;
    } else {
        // find and return the correlation coefficient
        coeff = cov / (stdDevX * stdDevY);
    }

    // Clamp the coefficient between -1 and 1
    if (coeff > 1.0) {
        coeff = 1.0;
    } else if (coeff < -1.0) {
        coeff = -1.0;
    }
    // coefficient close to 1 or -1 are similar closer to 0 is not similar
    return coeff;

}

void matching(const char* filename, const char* filename2, int type) {
    int searchWidth = 7;
    int maxDisparity = searchWidth;
    int minDisparity = 1;
    double focal = 520.0;
    double baseline = 60.0;
    double maxDistance = baseline*focal/minDisparity;
    double distance;
    double coeff;
    float xPoints[searchWidth * searchWidth];
    float yPoints[searchWidth * searchWidth];
    PPMImage* img = readPPM(filename, type);
    PPMImage* img2 = readPPM(filename2, type);

    // Allocate memory for resultantPoints
    unsigned char* resultantPoints = new unsigned char[img->width * img->height];
    unsigned char* disparityMap = new unsigned char[img->width * img->height];

    for (int i = 0; i < img->width; i += 1) {
        for (int j = 0; j < img->height; j += 1) {
            double bestCoeff = 0.0;
            int bestDisparity = 0;

            for(int d = 0 ; d < maxDisparity; d++){
                int index = 0;
                for (int k = 0; k < searchWidth; k++) {
                    for (int l = 0; l < searchWidth; l++) {
                        int imgIndex = (j + k) * img->width + (i + l);
                        int imgIndex2 = (j + k) * img2->width + (i + l - d);
                        if ((i + l) < img->width && (j + k) < img->height && (i + l - d) >= 0) {
                            xPoints[index] = img->data[imgIndex];
                            yPoints[index] = img2->data[imgIndex2];
                            index++;
                        } else {
                            xPoints[index] = 0;
                            yPoints[index] = 0;
                            index++;
                        }
                    }   
                }
                coeff = cocaCola(xPoints, searchWidth, searchWidth, yPoints, searchWidth, searchWidth, searchWidth * searchWidth);
                if (coeff > bestCoeff){
                    bestCoeff = coeff;
                    bestDisparity = d;
                }
            }   
            // Map the coefficient from [-1, 1] to [0, 255]
            if (bestDisparity == 0) {
                distance = maxDistance;
            } else {
                distance = baseline * focal / bestDisparity;
            }
            unsigned char mappedCoeff = static_cast<unsigned char>(255*(distance / maxDistance));
            unsigned char mappedDisparity = static_cast<unsigned char>(255*(bestDisparity / maxDisparity));
            resultantPoints[j * img->width + i] = mappedCoeff;
            disparityMap[j * img->width + i] = mappedDisparity;
        }
    }

    writePPM("depth.ppm", img->width, img->height, 255, 0, resultantPoints);
    writePPM("disparity.ppm", img->width, img->height, 255, 0, disparityMap);
    
    // Free allocated memory
    delete[] resultantPoints;
    delete[] disparityMap;
    freePPM(img);
    freePPM(img2);
}

int match2(){
    double start_time = omp_get_wtime();
    const int windowWidth = 15; //must be odd
    const int halfWindow = (windowWidth-1)/2;
    const int searchWidth = 71; //pixels must be odd
    const char* leftBW = "leftBW.ppm";
    const char* rightBW = "rightBW.ppm";
    const char* depthImageName = "depth.ppm";
    const char* disparityImageName = "disparity.ppm";
    PPMImage* leftImg;
    PPMImage* rightImg;
    int cols = 320;
    int rows = 240;
    int maxColor = 255;
    double baseLine = 60.0;
    double focalLength = 520.0;
    double maxDisparity = searchWidth;
    double minDisparity = 25.0;
    double maxDistance = baseLine*focalLength/minDisparity;
    double distance;
    double disparity = 0;
    int col;
    int index;
    int k;
    double maxCorr;
    //allocate memory for the output images
    unsigned char* depthImage = (unsigned char*) malloc(rows*cols * sizeof(unsigned char));
    unsigned char* disparityImage = (unsigned char*) malloc(rows*cols * sizeof(unsigned char));
    //read images
    leftImg = readPPM(leftBW,0);
    rightImg = readPPM(rightBW,0);

    for(int i = 0; i < rows*cols; i++){
        depthImage[i] = 0;
        disparityImage[i] = 0;
    }

    int num_threads = 8;
    omp_set_num_threads(num_threads);
    printf("Processing with %d threads\n", num_threads);

    #pragma omp parallel for schedule(dynamic, 8) private(col, index, k, disparity, distance, maxCorr)
    for(int row = halfWindow; row < rows-halfWindow; row++){
        float* leftPixels = new float[windowWidth * windowWidth];
        float* rightPixels = new float[windowWidth * windowWidth];
        double* pixelCorr = new double[searchWidth];

        for(int col = halfWindow; col < cols-halfWindow; col++){
            int index = 0;
            for(int k = -searchWidth/2; k < 0; k++){

                for(int i = -halfWindow; i <= halfWindow; i++){
                    for(int j = -halfWindow; j <= halfWindow; j++){
                        leftPixels[(i + halfWindow) * windowWidth + (j + halfWindow)] = leftImg->data[(row+i)*cols+(col+j)];
                        //printf("left: %7.1f", leftPixels[(i + halfWindow) * windowWidth + (j + halfWindow)]);
                        int rightIndex = (row+i)*cols+(col+k+j);
                        if(rightIndex >= 0 && rightIndex < rows*cols){
                        rightPixels[(i + halfWindow) * windowWidth + (j + halfWindow)] = rightImg->data[rightIndex];
                        }
                        else {
                            rightPixels[(i + halfWindow) * windowWidth + (j + halfWindow)] = 0;
                        }
                        //printf("right: %7.1f", rightPixels[(i + halfWindow) * windowWidth + (j + halfWindow)]);
                    }
                }
                pixelCorr[index] = cocaCola(leftPixels, windowWidth, windowWidth, rightPixels, windowWidth, windowWidth, windowWidth*windowWidth);
                index++;               
                
            }
                //compute disparity
                double maxCorr = -1.0;
                for(int k = 0; k < index; k++){
                    if(pixelCorr[k] > maxCorr){
                        maxCorr = pixelCorr[k];
                        disparity = abs(searchWidth/2 - k);
                    }
                }
            
                if(maxCorr > 0.5 && disparity < minDisparity){
                    distance = baseLine*focalLength/disparity;
                    depthImage[row*cols+col] = (unsigned char) (255 * (distance / maxDistance));
                    disparityImage[row * cols + col] = (unsigned char) (255 * (disparity / maxDisparity));
                    } else {
                    depthImage[row*cols+col] = 255;
                    disparityImage[row * cols + col] = 255;
                    }
        }
        delete[] leftPixels;
        delete[] rightPixels;
        delete[] pixelCorr;
    }

    writePPM(depthImageName, cols, rows, maxColor, 0, depthImage);
    writePPM(disparityImageName, cols, rows, maxColor, 0, disparityImage);

    double end_time = omp_get_wtime();
    printf("Processing completed in %.2f seconds\n", end_time - start_time);

    free(depthImage);
    free(disparityImage);
    freePPM(leftImg);
    freePPM(rightImg);
    return 0;
    }
    