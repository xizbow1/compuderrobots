#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

void vectorPrint(float* u , int rows){
  for(int i=0;i<rows;i++){
    printf( "%7.1f \n",u[i]);
  }
  printf("\n");

}

void vectorScale(float* u, int rows, float alpha , float* w){
  for(int i=0; i<rows; i++){
    u[i]=alpha*u[i];
  }
}

void vectorSubtract(float* u, float* v, int rows, float* w){
  for(int i=0; i<rows; i++){
    w[i]=u[i]-v[i];
  }
}

float vectorNorm(float* u, int rows){
  float sum = 0;
  for(int i=0; i<rows; i++){
    sum += u[i]*u[i];
  }
  return sqrt(sum);
}

float vectorDotProduct(float* u,float* v, int rows){
  float product = 0;
  for(int i=0; i<rows; i++){
    product += u[i]*v[i];
  } 
  return product;
}

void vectorScaling(float* v, int rows, float alpha, float* w){
  for(int i=0; i<rows; i++){
    w[i] = alpha*v[i];
  }
}

void matrixMultiply(float* a, int rows_a, int col_a, float* b, int rows_b, int col_b, float* c){
  if(col_a != rows_b){
    printf("Error: Matrix dimensions do not match\n");
    return;
  }
  
  for(int i = 0; i < rows_a; i++){
    for(int j = 0; j < col_b; j++){
      c[i * col_b + j] = 0;
      for(int k = 0; k < col_a; k++){
        c[i * col_b + j] += a[i * col_a + k] * b[k * col_b + j];
      }
      printf("%7.1f\n", c[i * col_b + j]);
    }
  }
}