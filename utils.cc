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
  printf("vectorScale\n");
  for(int i=0; i<rows; i++){
    w[i]=alpha*u[i];
  }
}

void vectorSubract(float* u, float* v, int rows, float* w){
  printf("vectorSubract\n");
  for(int i=0; i<rows; i++){
    w[i]=u[i]-v[i];
  }
}

void vectorNorm(float* u, int rows){
  printf("vectorNorm\n");
  float sum = 0;
  for(int i=0; i<rows; i++){
    sum += u[i]*u[i];
  }
  printf("%7.2f\n", sqrt(sum));
  printf("\n");
}

void vectorDotProduct(float* u,float* v, int rows, float* w){
  printf("vectorDotProduct\n");
  float product = 0;
  for(int i=0; i<rows; i++){
    product += u[i]*v[i];
  } 
  printf("%7.1f\n", product);
  printf("\n");
}

void vectorScaling(float* v, int rows, float alpha, float* w){
  printf("vectorScaling\n");
  for(int i=0; i<rows; i++){
    w[i] = alpha*v[i];
  }
}

void matrixMultiply(float* a, int rows_a,int col_a, float* b, int rows_b, int col_b, float* c){
  printf("matrixMultiply\n");
  for(int i=0; i<rows_a; i++){
    for(int j=0; j<col_b; j++){
      c[j] = a[i]*b[j] + a[i+rows_a]*b[j+col_b];
      printf("%7.1f\n", c[j]);
    }
  }

}