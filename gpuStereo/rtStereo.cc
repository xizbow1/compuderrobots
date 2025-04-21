#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <string>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "stereoDepth.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

int fps = 15; // in frames per sec
int frameDelay = 1000/(2*fps); // in millisec 
double maxDistance = 3000.0; // mm
int rows  = 480;
int cols  = 640;
Mat depthImage = Mat::zeros(rows,cols, CV_8UC1);

//Read rectification lookup tables
Mat map1x,map1y,map2x,map2y;
FileStorage fs("lookupTables.xml",FileStorage::READ);
fs["Map1x"]>>map1x;
fs["Map1y"]>>map1y;
fs["Map2x"]>>map2x;
fs["Map2y"]>>map2y;
fs.release();

if( map1x.empty()) cout << "Empty 1x lookup table"<<endl;
if( map1y.empty()) cout << "Empty 1y lookup table"<<endl;
if( map2x.empty()) cout << "Empty 2x lookup table"<<endl;
if( map2y.empty()) cout << "Empty 2y lookup table"<<endl;

//correct any remaining alignment errors
float offset = 14.0;
float currentRow;
for(int row = 0; row < rows; row++){
    for(int col = 0; col < cols; col++){
        currentRow = map2y.at<float>(row,col);
        if(currentRow+offset < 0 || currentRow+offset > rows){
            map2y.at<float>(row,col) = currentRow;
        } else {
            map2y.at<float>(row,col) = currentRow + offset;
        }
    }
}

// GStreamer pipeline for Jetson Nano with IMX219-83 cameras
 string left_cam_pipeline  = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=640, height=480, framerate="+to_string(fps)+
                              "/1 ! nvvidconv flip-method=2 ! video/x-raw, format=GRAY8 !  appsink drop=1";

 string right_cam_pipeline = "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=640, height=480, framerate="+to_string(fps)+
                              "/1 ! nvvidconv flip-method=2 ! video/x-raw, format=GRAY8 !  appsink drop =1";
    
// Open both cameras
    VideoCapture capL(left_cam_pipeline, CAP_GSTREAMER);
    VideoCapture capR(right_cam_pipeline,CAP_GSTREAMER);


    if (!capL.isOpened() || !capR.isOpened()) {
        cerr << "Error: Could not open stereo cameras." << endl;
        return -1;
    }


    Mat leftFrame, rightFrame;

    cout << " width \n" << capL.get(CAP_PROP_FRAME_WIDTH)<<endl;
    cout << " height  \n" << capL.get(CAP_PROP_FRAME_HEIGHT)<<endl;
    cout << " format \n" << capL.get(CAP_PROP_FORMAT)<<endl;
    cout << " fps \n" << capL.get(CAP_PROP_FPS)<<endl;
    
    // Real-time loop for capturing frames
    while (true) {

        // Capture frames from both cameras
        capL >> leftFrame;
        capR >> rightFrame;

        if (leftFrame.empty() || rightFrame.empty()) {
            cerr << "Error: Empty frame detected!" << endl;
            break;
        }



      // Apply rectification
      Mat rectifiedLeft, rectifiedRight, both;
      remap(leftFrame, rectifiedLeft, map1x, map1y, INTER_LINEAR);
      remap(rightFrame, rectifiedRight, map2x, map2y, INTER_LINEAR);


      // Compute depth image using GPU
      stereoDepth(&rectifiedLeft, &rectifiedRight, &depthImage, maxDistance, rows, cols);


      // dispaly depth map
      imshow("Depth",depthImage);
      hconcat(rectifiedLeft, rectifiedRight,both);
      imshow("Left and Right",both);
  

      // pause
      waitKey(frameDelay) ;


    }

    // Release resources
    capL.release();
    capR.release();
    destroyAllWindows();

    return 0;
}


