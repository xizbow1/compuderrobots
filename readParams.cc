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

int readParams (string fileName) {


ifstream infile;
string line;
int value;
int imageWidth,imageHeight,numPoints;
float zc;

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


infile.close ();

return numPoints;
}
