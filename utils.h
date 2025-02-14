#ifndef UTILS_H
#define UTILS_H


using namespace std;

void vectorPrint(float* u , int rows); 

void vectorScale(float* u, int rows, float alpha , float* w); 

void vectorSubtract(float* u, float* v, int rows, float* w);

float vectorNorm(float* u, int rows);

float vectorDotProduct(float* u,float* v, int rows);

void vectorScaling(float* v, int rows, float alpha, float* w);

void matrixMultiply(float* a, int rows_a, int cols_a, float* b, int rows_b,int cols_b, float* c);


#endif

