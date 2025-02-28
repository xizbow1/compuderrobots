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
        sqX += x[rowX * colX + i] * x[rowX * colX + i];
        sqY += y[rowY * colY + i] * y[rowY * colY + i];

        mX += x[rowX * colX + i];
        mY += y[rowY * colY + i];

        prod += x[rowX * colX + i] * y[rowY * colY + i];
    }

    sqX = sqX/n;
    sqY = sqY/n;
    mX = mX/n;
    mY = mY/n;
    
    cov = (prod/n) - (mX*mY);

    stdDevX = sqrt(sqX - (mX*mX));
    stdDevY = sqrt(sqY - (mY*mY));

    coeff = cov/(stdDevX*stdDevY);

    return coeff;

}

void matching(const char* filename, const char* filename2, int type){
    int searchWidth = 3;
    PPMImage* img = readPPM(filename, type);
    PPMImage* img2 = readPPM(filename2, type);
    for(int i = 0; i < img->width; i += searchWidth){ // To move by searchWidth bytes
        int xPoints[searchWidth * searchWidth];
        int yPoints[searchWidth * searchWidth];
        for(int j = 0; j < searchWidth; j += searchWidth){
            for(int k = 0: k < searchWidth; k++){
                xPoints[j + k] = img->data[(img->width * img->height) + k];
                yPoints[j + k] = img2->data[(img2->width * img2->height) + k];
            }
        }
    }
    writePPM(depth.ppm, )
}