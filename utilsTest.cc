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



int rows = 3;
float alpha = 2.0;

vectorPrint(u, rows);

vectorScale(u,rows,alpha,v);
vectorPrint(v,rows);

}
