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

void matrixPrint(float* matrix, int rows, int cols) {
    printf("number of rows and cols,%d %d \n", rows, cols);
    for (int row = 0; row < rows; row++) {

        for (int col = 0; col < cols; col++) {
            printf("%6.5f ",matrix[row*cols + col]);
        }
        printf("\n");

    }
    printf("\n");
}

void matrixProduct(float* a, int rows_a, int cols_a, float* b, int rows_b, int cols_b, float* c){
 int index = 0;
 int rows  = rows_a;
 int cols  = cols_b;

 if(cols_a == rows_b){
 for(int row = 0; row < rows; row++){
   for(int col = 0; col < cols; col++){
       index = row*cols + col;
       c[index] = 0.0; 
       for(int k = 0; k < cols_a; k++){
         c[index]=c[index]+a[row*cols_a + k]*b[col+k*cols_b];
       }
   }

 }

 }else{
	 printf("Can't multiply. Dimensions incorrect \n");
	 exit(0);
 }

}

void matrixTimesVector(float* a, int rows, int cols, float* v, int rows_v, float* w){
if(cols==rows_v){
   for( int row = 0; row < rows; row++){
                w[row]=0.0;
		for( int col = 0;col < cols;col++){
                   w[row]=w[row]+a[row*cols+col]*v[col];
		}
   } 
}else{
	printf(" dimensions don't match \n");
	exit(0);
}

}

void matrixTranspose(float* a, int rows, int cols, float* aTranspose){
 for( int row = 0; row < rows; row++){
		for( int col = 0;col < cols;col++){

		    aTranspose[col*rows+row] = a[row*cols+col];
		}
		 
 }
}

void matrixQR(float* a, int rows, int cols, float* q, float* r) {
    // Initialize temp array u
    float* u = new float[rows];

    // Initialize Q and R to zero
    for (int i = 0; i < rows * cols; i++) {
        q[i] = 0.0;
        r[i] = 0.0;
    }

    // Gram-Schmidt process
    for (int i = 0; i < cols; i++) {
        // Copy column i of A to u
        for (int j = 0; j < rows; j++) {
            u[j] = a[j * cols + i];
        }

        // Orthogonalize u against previous columns of Q
        for (int j = 0; j < i; j++) {
            float dotP = 0.0;
            for (int k = 0; k < rows; k++) {
                dotP += q[k * cols + j] * u[k];
            }
            r[j * cols + i] = dotP;
            for (int k = 0; k < rows; k++) {
                u[k] -= dotP * q[k * cols + j];
            }
        }

        // Compute the norm of u
        float norm = 0.0;
        for (int j = 0; j < rows; j++) {
            norm += u[j] * u[j];
        }
        norm = sqrt(norm);

        // Normalize u and store in Q
        r[i * cols + i] = norm;
        for (int j = 0; j < rows; j++) {
            if(norm == 0){
            q[j * cols + i] = 0;
            }else{
            q[j * cols + i] = u[j] / norm;
            }
        }
    }

    delete[] u;
}


void matrixExtractCol(float*a, int rows, int cols, int col, float* column){
    int index;
    for(int row=0;row<rows;row++){
	index = col + row*cols;
        column[row] = a[index];
    }
}


void matrixBackSubstitution(float* R, int rows, int cols, float* d, float* p){
    //solve Rp=d for an uppertriangular matrix R
    // using back substitution

    for (int j = cols-1; j > -1; j--){
        p[j]=d[j];
    for(int k = j+1; k < cols; k++){
        p[j]= p[j] - R[j*cols + k]*p[k];
    }
    p[j]=p[j]/R[j*cols+j];
    }

}

void matrixUpperTriangularInverse(float* A, int rows, int cols, float* invA){
// This code computes the inverse of a non-singular upper triangular 3x3 matrix.
    if(rows !=3 || cols !=3){
        printf(" Only 3x3 full rank upper triangle matricies can be inverted with this code\n");
            exit(0);
    }

    for(int row = 0; row < rows; row++){
    for(int col= 0;col < cols; col++){
        invA[ row*cols + col]  = 0.0;
    }
    }

    invA[0*cols+0]=1.0/A[0*cols+0];
    invA[1*cols+1]=1.0/A[1*cols+1];
    invA[2*cols+2]=1.0/A[2*cols+2];

    invA[0*cols+1]=-A[0*cols+1]*invA[1*cols+1]/A[0*cols+0];
    invA[1*cols+2]=-A[1*cols+2]*invA[2*cols+2]/A[1*cols+1];
    invA[0*cols+2]=-( A[0*cols+1]*invA[1*cols+2] + A[0*cols+2]*invA[2*cols+2] )/A[0*cols+0];
}

void matrixInternalCameraParameters(float* p,int rows,int cols,float* k){

    float pSub[9];
    float r[9];
    float rInv[9];
    float q[9];
    float qT[9];
    float b[9];

    // extract a 3x3 sub matrix of p
    for (int row=0; row < rows; row++){
        for (int col=0; col<cols-1;col++){
                pSub[row*(cols-1)+col]=p[row*cols+col];
        }
    }

    printf("3x3 submatrix of p\n");
    matrixPrint(pSub,3,3);

    // find the inverse of pSub with QR decomposition
    // Its inverse is B=RInv*QTranspose
    matrixQR(pSub,3,3,q,r);
    matrixTranspose(q,3,3,qT);
    matrixUpperTriangularInverse(r,3,3,rInv);
    matrixProduct(rInv,3,3,qT,3,3,b);

    //QR decompose the matrix B
    matrixQR(b,3,3,q,r);

    int rows_k = rows;
    int cols_k = rows_k;

    int rows_r = rows;
    int cols_r = rows_r;

    //diagonals of calibration matirx
    k[0*cols_k +0] = 1/r[0*cols_r+0];
    k[1*cols_k +1] = 1/r[1*cols_r+1];
    k[2*cols_k +2] = 1.0;
    // off diagonals of the calibration matrix
    k[0*cols_k +2] = -r[0*cols_r +2]/r[0*cols_r+0];
    k[1*cols_k +2] = -r[1*cols_r +2]/r[1*cols_r+1];



}
