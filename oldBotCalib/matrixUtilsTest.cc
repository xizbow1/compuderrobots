#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <string>
#include <sstream>
#include "matrixUtils.h"

int main(){

float a [] = {
              1,2,3,
              4,5,6
            };
int rows = 2;
int cols = 3;

float aT[rows*cols];

matrixPrint(a, rows, cols);

matrixTranspose(a, rows, cols, aT);

}