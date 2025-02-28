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


