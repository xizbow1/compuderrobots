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

void readData (string fileName, float u[],float v[],float x[] , float y[], float z[], float zc[]) {


ifstream infile;
string line;
int value;
int imageWidth,imageHeight,numPoints;

//open file
infile.open (fileName);

//read first line to get image parameters
getline(infile,line); // Saves the line in STRING.

istringstream iss(line);

value=0;
do{
	string sub;
	iss >> sub;
	if(value==0) imageWidth		=   atoi(sub.c_str());
	if(value==1) imageHeight	=   atoi(sub.c_str());	
        if(value==2) numPoints      	=   atoi(sub.c_str());
value++;
	
}while(iss);

printf("image params %d %d %d \n",imageWidth,imageHeight,numPoints);

//read data
for(int i=0; i<numPoints; i++){

  getline(infile,line); // Saves the line in STRING.

  istringstream iss(line);

  value=0;
  do{
	string sub;
	iss >> sub;
	if(value==0) u[i]      	=   imageWidth  - atof(sub.c_str());
        if(value==1) v[i]      	=   atof(sub.c_str());
	if(value==2) x[i] 	=   atof(sub.c_str())*25.0;
	if(value==3) y[i]      	=   atof(sub.c_str())*25.0;
        if(value==4) z[i]       =   atof(sub.c_str())*25.0;	
	if(value==5) zc[i]      =   atof(sub.c_str());
  value++;
  }while(iss);


printf("calibration data  %f %f %f %f %f %f \n",u[i],v[i],x[i],y[i],z[i],zc[i]);
}

infile.close ();


}
