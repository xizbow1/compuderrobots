#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <string>
#include <sstream>
#include "utils.h"
#include "imageUtils.h"

using namespace std;

double cocaCola(float* x, int rowX, int colX, float* y, int rowY, int colY, int n) {
    int prod = 0;
    int sqX = 0;
    int sqY = 0;
    int mX = 0;
    int mY = 0;
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

    // find and return the correlation coefficient
    coeff = cov / (stdDevX * stdDevY);

    // coefficient close to 1 or -1 are similar closer to 0 is not similar
    return coeff;

}

void matching(const char* filename, const char* filename2, int type) {
    int searchWidth = 3;
    float xPoints[searchWidth * searchWidth];
    float yPoints[searchWidth * searchWidth];
    PPMImage* img = readPPM(filename, type);
    PPMImage* img2 = readPPM(filename2, type);

    // Allocate memory for resultantPoints
    unsigned char* resultantPoints = new unsigned char[img->width * img->height];

    for (int i = 0; i < img->width; i += searchWidth) {
        for (int j = 0; j < img->height; j += searchWidth) {
            for (int k = 0; k < searchWidth; k++) {
                int index = j * img->width + i + k;
                if (index < img->width * img->height) {
                    xPoints[k] = img->data[index];
                    yPoints[k] = img2->data[index];
                }
            }
            resultantPoints[j * img->width + i] = static_cast<unsigned char>(cocaCola(xPoints, searchWidth, searchWidth, yPoints, searchWidth, searchWidth, searchWidth * searchWidth));
        }
    }

    writePPM("depth.ppm", img->width, img->height, 255, 0, resultantPoints);

    // Free allocated memory
    delete[] resultantPoints;
    freePPM(img);
    freePPM(img2);
}