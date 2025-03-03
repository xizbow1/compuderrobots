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


double cocaCola(float* x,int rowX,int colX, float* y, int rowY, int colY, int n){

    int prod = 0;
    int sqX = 0;
    int sqY = 0;
    int mX = 0;
    int mY = 0;
    double cov = 0.0;
    double coeff = 0.0;
    double stdDevX = 0.0;
    double stdDevY = 0.0;

    for(int i = 0; i < n; i++){
        int row = i / colX;
        int col = i % colX;

        //find the squares of both x and y
        sqX += x[row * colX + col] * x[row * colX + col];
        sqY += y[row * colY + col] * y[row * colY + col];

        //find the sum of x and y
        mX += x[row * colX + col];
        mY += y[row * colY + col];

        //find the product of x and y
        prod += x[row * colX + col] * y[row * colY + col];
    }

    //find the mean of x and y squared
    sqX = sqX/n;
    sqY = sqY/n;

    //find the mean of both mX and mY
    mX = mX/n;
    mY = mY/n;
    
    //find the convergence 
    cov = (prod/n) - (mX*mY);

    //find the standard deviantion
    stdDevX = sqrt(sqX - (mX*mX));
    stdDevY = sqrt(sqY - (mY*mY));

    //find and return the correlation coefficent
    coeff = cov/(stdDevX*stdDevY);

    //coefficient close to 1 or -1 are similar closer to 0 is not similar
    return coeff;
    

}

void matching(const char* filename, const char* filename2, int type){
    int searchWidth = 3;
    float xPoints[searchWidth * searchWidth];
    float yPoints[searchWidth * searchWidth];
    unsigned char* resultantPoints;
    int i = 0;
    int j = 0;
    PPMImage* img = readPPM(filename, type);
    PPMImage* img2 = readPPM(filename2, type);
    for(i = 0; i < img->width; i += searchWidth){
        for(j = 0; j < searchWidth; j += searchWidth){
            for(int k = 0; k < searchWidth; k++){
                xPoints[j + k] = img->data[(img->width * img->height) + k];
                yPoints[j + k] = img2->data[(img2->width * img2->height) + k];
            }
        }
    }
    for(i = 0; i < img->width; i++){
        resultantPoints[i] = static_cast<unsigned char>(cocaCola(xPoints, searchWidth, searchWidth, yPoints, searchWidth, searchWidth, searchWidth * searchWidth));
    }

    writePPM("depth.ppm", i, j, 255, 1, resultantPoints);
}