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
#include "utils.h"
#include "matrixUtils.h"

using namespace std;

// This code was written by Ken Owens 2/11/2025
// to solve Ap=b using QR decomposition.
// A = mT*m is a real symmetric matrix
// and p is the camera projection matrix.
// Matrix M is composed of world calibration data
// and the vector b is composed of pixel calibration data.
// We sove A=QR which gives QRp=b and reduces to Rp=QT*b. 
// which we sove by back substitution since R is upper triagular.

int main(){

int numPoints = 0;
string inputFile;

// read the camera calibration parameters 
cout << "Enter the name of the calibration file: ";
    getline(std::cin, inputFile); // Read a line

numPoints = readParams(inputFile);

printf( "Number of Points %d \n",numPoints);

if(numPoints<12){
	printf("Not enough points. Min of 12 are needed \n");
	exit(0);
}

//define input vectors
float   u[numPoints];
float   v[numPoints];
float   x[numPoints];
float   y[numPoints];
float   z[numPoints];
float  zc[numPoints];

//read calibration data
readData(inputFile, u, v, x ,y, z, zc);

// build m matrix of world coordinates three rows at a time
// for each data point

const int rows= 3*numPoints;
const int cols = 12;

float   m[rows*cols]={0.0};
float   mTranspose[cols*rows];
float   a[cols*cols];
float   c[rows];
float   b[cols];
float   d[cols];
float   q[cols*cols];
float   qT[cols*cols];
float   r[cols*cols];
float   p[cols];
float   k[9] = {0.0};
float   test[cols*cols];

// initialize the M matrix 
int row=0;
int col=0;
for(int i=0;i<numPoints;i++){
col=0;
  for (int j=0;j<3;j++){
     m[row*cols+col] = x[i];
     col++;
     m[row*cols+col] = y[i];
     col++;
     m[row*cols+col] = z[i];
     col++;
     m[row*cols+col] = 1.0;
     col++;

     printf(" row %d",row);
     for(int k = 0; k<cols;k++) printf(" %f",m[row*cols+k]);
     printf("\n");

     row = row +1;

  }

}

//compute the right hand side vector b
row=0;
for(int i=0;i<numPoints;i++){
 c[row]=u[i]*zc[i];
 row++;
 c[row]=v[i]*zc[i];
 row++;
 c[row]=zc[i];
 row++; 
}

printf("M matrix \n");
matrixPrint(m,rows,cols);

printf("c right hand side vector \n");
vectorPrint(c,rows);

//compute m transpose mT
matrixTranspose(m,rows,cols,mTranspose);

// compute matrix a = mT*m
matrixProduct(mTranspose,cols,rows,m,rows,cols,a);
printf("Matrix A \n");
matrixPrint(a,cols,cols);

// compute QR decomposition of matrix A
matrixQR(a,cols,cols,q,r);

printf("Q matrix \n");
matrixPrint(q,cols,cols);

printf("R matrix \n");
matrixPrint(r,cols,cols);

printf("QR  This must equal matrix A \n");
matrixProduct(q,cols,cols,r,cols,cols,test);
matrixPrint(test,cols,cols);

//compute the transpose of q
matrixTranspose(q,cols,cols,qT);

// check qT*q should equal the identity
printf("q*qT should be a 12x12 identity matrix \n");
matrixProduct(qT,cols,cols,q,cols,cols,test);
matrixPrint(test,cols,cols);

// compute the righthand side vector b=mT*c
matrixTimesVector(mTranspose,cols,rows,c,rows,b);
printf("b=mT*c should have 12 rows \n");
vectorPrint(b,cols);

// compute the righthand side vector d=qT*b
matrixTimesVector(qT,cols,cols,b,cols,d);

// compute the projection matrix with back substitution
matrixBackSubstitution(r,cols,cols,d,p);
printf("projection matrix \n");
matrixPrint(p,3,4);

matrixInternalCameraParameters(p,3,4,k);
printf("calibration matrix \n");
matrixPrint(k,3,3);


return 0;

}
