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

// function to output standard deviation given x and y
double std_dev(double x, double y) {
  double mean = (x + y) / 2;
  return sqrt(((x - mean) * (x - mean) + (y - mean) * (y - mean)) / 2);
}

void correlationCoeffecient(float* x,int rowX,int colX, float* y, int rowY, int colY, int n, float* c){

  int prod = 0;
  int sqX = 0;
  int sqY = 0;
  int mX = 0;
  int mY = 0;

  for(int i = 0; i < n; i++){
      sqX += x[rowX * colX + i] * x[rowX * colX + i];
      sqY += y[rowY * colY + i] * y[rowY * colY + i];

      mX += x[rowX * colX + i];
      mY += y[rowY * colY + i];

      prod = x[rowX * colX + i] * x[rowY * colY + i];
  }

  sqX = sqX/n;
  sqY = sqY/n;
  mX = mX/n;
  mY = mY/n;

}