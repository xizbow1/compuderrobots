#ifndef UTILS_H
#define UTILS_H


using namespace std;

void vectorPrint(float* u , int rows); 

void vectorScale(float* u, int rows, float alpha , float* w); 

void vectorSubract(float* u, float* v, int rows, float* w);

void vectorNorm(float* u, int rows);

void vectorDotProduct(float* u,float* v, int rows, float* w);

void vectorScaling(float* v, int rows, float alpha, float* w);

void matrixMultiply(float* a, int rows_a, int cols_a, float* b, int rows_b,int cols_b, float* c);


#endif

