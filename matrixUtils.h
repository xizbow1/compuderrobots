#ifndef MATRIXUTILS_H
#define MATRIXUTILS_H


using namespace std;

void matrixPrint(float* matrix, int rows, int cols);  

void matrixProduct(float* a, int rows_a, int cols_a, float* b, int rows_b, int cols_b, float* c); 

void matrixTimesVector(float* a, int rows, int cols, float* v, int rows_v, float* w);

void matrixTranspose(float* a, int rows, int cols, float* aTranspose);

void matrixQR(float* p,int rows, int cols, float* q, float* r);

void matrixExtractCol(float*a, int rows, int cols, int col, float* column);

void matrixBackSubstitution(float* R, int rows, int cols, float* d, float* p);

void matrixUpperTriangularInverse(float* A, int rows, int cols, float* invA);

void matrixInternalCameraParameters(float* p,int rows,int cols,float* k);

#endif

