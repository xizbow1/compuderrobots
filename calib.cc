#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <string>
#include <sstream>
#include "readParams.h"
#include "readData.h"

using namespace std;

int main(){

int numPoints = 0;
string inputFile;

cout << "Enter the name of the calibration file: ";
    getline(std::cin, inputFile); // Read a line

numPoints = readParams(inputFile);

printf( "Number of Points %d \n",numPoints);

if(numPoints<12){
	printf("Not enough points. Min of 12 are needed \n");
	exit(0);
}

int rows = 3*numPoints;
int cols = 12;

float   u[rows];
float   v[rows];
float   x[rows];
float   y[rows];
float   z[rows];
float  zc[rows];

readData(inputFile, u, v, x ,y, z, zc);

return 0;

}
