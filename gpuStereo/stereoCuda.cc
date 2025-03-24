#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "stereoDepth.h"
#include <time.h>

using namespace cv;
using namespace std;

int main(){

int rows = 480;
int cols = 640;
double maxDistance = 2000.0;
clock_t start,end;
double time_used;

//read images
Mat left  = imread("leftBW.ppm", IMREAD_GRAYSCALE);
Mat right = imread("warpedImg.ppm", IMREAD_GRAYSCALE);
Mat depth = Mat::zeros(rows,cols, CV_8UC1);

start = clock();
stereoDepth(&left, &right, &depth, maxDistance, rows, cols);
end = clock();

time_used = ( (double)(end-start))/CLOCKS_PER_SEC;
printf("Time in seconds  %.6f \n",time_used);
imshow("depth",depth);
    waitKey(0);  // 0 means wait indefinitely until a key is pressed

return 0;
}
