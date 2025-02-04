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
  printf("%7.1f\n", sum);
  printf("\n");
}

void vectorDotProduct(float* u,float* v, int rows, float* w){
  printf("vectorDotProduct\n");
  for(int i=0; i<rows; i++){
    w[i]=u[i]*v[i];
  } 
}

void vectorScaling(){
  printf("vectorScaling\n");
}