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

int rows = 3;

float alpha = 2.0;

vectorPrint(u, rows);

vectorScale(u,rows,alpha,w);
vectorPrint(w,rows);

vectorSubract(u,v,rows,w);
vectorPrint(w,rows);

vectorNorm(u,rows);

vectorDotProduct(u,v,rows,w);

vectorScaling();

}
