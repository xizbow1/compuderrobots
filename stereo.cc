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


double correlationCoeffecient(float* x,int rowX,int colX, float* y, int rowY, int colY, int n){
    //initalize variables
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
        //find the squares of both x and y
        sqX += x[rowX * colX + i] * x[rowX * colX + i];
        sqY += y[rowY * colY + i] * y[rowY * colY + i];

        //find the sum of x and y
        mX += x[rowX * colX + i];
        mY += y[rowY * colY + i];

        //find the product of x and y
        prod += x[rowX * colX + i] * y[rowY * colY + i];
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
