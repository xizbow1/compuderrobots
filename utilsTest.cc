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

int main(){

float u[] = {
	     1,
	     2,
	     3
           };

float v[] = {
             4,
             5,
             6 
           };

float w[3];

float a [] = {
              2,1,
              1,2
            };
int rows_a = 2;
int cols_a = 2;

float b[] = {
             1,2,3,4,
             5,6,7,8
           };
int rows_b = 2;
int cols_b = 4;

float c[] = {
              0,0,0,0,
              0,0,0,0
            };
int rows_c = 2;
int cols_c = 4;

int rows = 3;

float alpha = 2.0;

vectorPrint(u, rows);

vectorScale(u,rows,alpha,w);
vectorPrint(w,rows);
/*
Testing vectorSubract
Passing u,v,rows and w as arguments
u = {1,2,3}, v = {4,5,6}
w = {1-4, 2-5, 3-6} = {-3, -3, -3}
*/
vectorSubtract(u,v,rows,w);
vectorPrint(w,rows);

/*
Testing vectorNorm
Passing u and rows as arguments
u = {1,2,3}
sum = 1^2 + 2^2 + 3^2 = 14
sqrt(14) = 3.74
*/
printf("%f",vectorNorm(u,rows));


/*
Testing vectorDotProduct
Passing u,v,rows and w as arguments
u = {1,2,3}, v = {4,5,6}
product = 1*4 + 2*5 + 3*6 = 32
*/
vectorDotProduct(u,v,rows);

/*
Testing vectorScaling
Passing v,rows,alpha and w as arguments
v = {4,5,6}, alpha = 2
w = {4*2, 5*2, 6*2} = {8, 10, 12}
*/
vectorScaling(v,rows,alpha,w);
vectorPrint(w,rows);

matrixMultiply(a,rows_a,cols_a,b,rows_b,cols_b,c);

}
