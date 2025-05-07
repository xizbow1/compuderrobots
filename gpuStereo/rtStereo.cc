#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <string>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "stereoDepth.h"
#include "stereoObstacles.h"
#include "serialUtils.h"

using namespace cv;
using namespace std;

int portID;
volatile bool exitRequested = false;

void intHandler(int sig){
    serialPortWrite("STP\n",portID);
    exitRequested = true; // Set flag instead of exiting immediately
    printf("\nStopping robot and exiting...\n");
}

int main(int argc, char** argv) {

// Dispaly Parameters
int fps = 60; // in frames per sec
int frameDelay = 1000/(2*fps); // in millisec 
double maxDistance = 1000.0; // mm
double maxDisparity = 64; // pixels
int rows  = 480;
int cols  = 640;
Mat depthImage = Mat::zeros(rows,cols, CV_8UC1);


// Serial parameters
const int cmdLength = 7;
char cmd[cmdLength];
int bytesWritten;
const char* strCmd;
const char* moveCmd;

portID = serialPortOpen();
if(portID<0){
    printf("Error opening serial port \n");
    exit(0);
}

// Obstacle Parameters
Mat obstacleImage = Mat::zeros(rows,cols, CV_8UC1);
int zone0End = cols / 6;
int zone1End = 2*(cols/6);
int zone2End = 4*(cols/6);
int zone3End = 5*(cols/6);
int zone4End = cols;
int obstacleThreshold = 2000;

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
    
    signal(SIGINT, intHandler);

    // Real-time loop for capturing frames
    while (!exitRequested) {

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
    stereoDepth(&rectifiedLeft, &rectifiedRight, &depthImage, maxDisparity, rows, cols);
    // Compute obstacles image using GPU
    stereoObstacles(&depthImage, &obstacleImage, maxDistance, rows, cols);


    // Zones are split from left to right 0 - 4
    // On the robot left to right will be reversed
    // The robot's right will be zone 0
    // The robot's left will be zone 4
    /*
    bool zone0Clear = true;
    bool zone1Clear = true;
    bool zone2Clear = true;
    bool zone3Clear = true;
    bool zone4Clear = true;
    */

    int zone0Count = 1;
    int zone1Count = 1;
    int zone2Count = 1;
    int zone3Count = 1;
    int zone4Count = 1;
    int pixel;
    int startRow = rows * 1/3;
    
    for(int row = startRow; row < rows; row++){
        for(int col = 0; col < cols; col++){
            pixel = (int)(obstacleImage.data[row*cols+col]);
            if(col >= 0 && col < zone0End && pixel > 0) zone0Count++;           // Far Left Zone
            if(col >= zone0End && col < zone1End && pixel > 0) zone1Count++;    // Mid Left Zone
            if(col >= zone1End && col < zone2End && pixel > 0) zone2Count++;    // Middle Zone
            if(col >= zone2End && col < zone3End && pixel > 0) zone3Count++;    // Mid Right Zone
            if(col >= zone3End && col < zone4End && pixel > 0) zone4Count++;    // Far Right Zone
        }
    }
    
    /*
    Mat roi_zone0 = obstacleImage(Rect(0,0, zone0End, rows));
    zone0Count = countNonZero(roi_zone0);

    Mat roi_zone1 = obstacleImage(Rect(0,0, zone1End, rows));
    zone1Count = countNonZero(roi_zone1);

    Mat roi_zone2 = obstacleImage(Rect(0,0, zone2End, rows));
    zone2Count = countNonZero(roi_zone2);
    
    Mat roi_zone3 = obstacleImage(Rect(0,0, zone3End, rows));
    zone3Count = countNonZero(roi_zone3);

    Mat roi_zone4 = obstacleImage(Rect(0,0, zone4End, rows));
    zone4Count = countNonZero(roi_zone4);
    */

    printf("zone0: %d, zone1: %d, zone2: %d, zone3: %d, zone4: %d\n", zone0Count, zone1Count, zone2Count, zone3Count, zone4Count);
    dodgeObstacles(zone0Count, zone1Count, zone2Count, zone3Count, zone4Count, portID);
    /*
    // Determine if zone count is above threshold
    if(zone0Count > obstacleThreshold) zone0Clear = false;
    else zone0Clear = true;
    if(zone1Count > obstacleThreshold) zone1Clear = false;
    else zone1Clear = true;
    if(zone2Count > obstacleThreshold) zone2Clear = false;
    else zone2Clear = true;
    if(zone3Count > obstacleThreshold) zone3Clear = false;
    else zone3Clear = true;
    if(zone4Count > obstacleThreshold) zone4Clear = false;
    else zone4Clear = true;

    // zone0 - zone1 - zone2 - zone3 - zone4
    if(zone1Clear && zone2Clear && zone3Clear){
        strCmd = "STR090\n";
        moveCmd = "BWD080\n";
    }
    if(!zone0Clear && !zone1Clear && !zone2Clear && !zone3Clear && !zone4Clear){
        strCmd = "STR90\n";
        moveCmd = "BWD080\n";
    }
    if(!zone1Clear && !zone2Clear && !zone3Clear && zone0Clear){
        strCmd = "STR110\n";
        moveCmd = "BWD080\n";
    }
    if(!zone1Clear && !zone2Clear && !zone3Clear && zone4Clear){
        strCmd = "STR070\n";
        moveCmd = "BWD080\n";
    }
    if(!zone1Clear && !zone2Clear && zone3Clear){
        strCmd = "STR120\n";
        moveCmd = "FWD080\n";
    }
    if(zone1Clear && !zone2Clear && !zone3Clear){
        strCmd = "STR060\n";
        moveCmd = "FWD080\n";
    }
    if(!zone0Clear && zone1Clear){
        strCmd = "STR080\n";
        moveCmd = "FWD080";
    }
    if(!zone4Clear && zone3Clear){
        strCmd = "STR100\n";
        moveCmd = "FWD080\n";
    }

    // Write to serial port the driving commands
    //printf("STR: %s, Move: %s\n", strCmd, moveCmd);
    bytesWritten = serialPortWrite(moveCmd,portID);
    bytesWritten = serialPortWrite(strCmd,portID);
    */

    // Drawing obstacle zones border lines 
    // Zone 0 far left 
    line(obstacleImage, Point(zone0End, 0), Point(zone0End, rows-1), Scalar(255), 1);
    // Zone 1 mid left
    line(obstacleImage, Point(zone1End, 0), Point(zone1End, rows-1), Scalar(255), 1);
    // Zone 2 middle
    line(obstacleImage, Point(zone2End, 0), Point(zone2End, rows-1), Scalar(255), 1);
    // Zone 3 mid right
    line(obstacleImage, Point(zone3End, 0), Point(zone3End, rows-1), Scalar(255), 1);
    // Zone 4 far right
    // End of zone 4 is the edge of the image so no border line is necessary

    //Smoothing depth image
    Mat medianDepth, filteredDepth, medianObstacles;
    medianBlur(depthImage, medianDepth, 5);
    medianBlur(obstacleImage, medianObstacles, 5);
    //GaussianBlur(medianDepth, filteredDepth, Size(5,5), 0);

    // Display depth map
    imshow("Depth", medianDepth);
    // Display obstacle map
    imshow("Obstacles",medianObstacles);
    // Dispaly rectified images 
    //hconcat(rectifiedLeft, rectifiedRight,both);
    //imshow("Left and Right",both);
  
    // Pause
    waitKey(frameDelay);

        // Check if exit was requested during waitKey
        if (exitRequested) {
            break;
        }
    }

    // Send a final stop command to ensure robot stops
    bytesWritten = serialPortWrite("STP\n", portID);
    
    // Close serial port
    if(serialPortClose(portID)< 0){
        printf("Could not close serial port \n");
        exit(0);
    } else{
        printf("Serial port closed \n");
    }
   
    // Release resources
    capL.release();
    capR.release();
    destroyAllWindows();

    return 0;
}

void dodgeObstacles(int zone0Count, int zone1Count, int zone2Count, int zone3Count, int zone4Count, int portID){
    bool zone0Clear = true;
    bool zone1Clear = true;
    bool zone2Clear = true;
    bool zone3Clear = true;
    bool zone4Clear = true;
    const char* strCmd;
    const char* moveCmd;
    int bytesWritten;
    int obstacleThreshold = 2000;

    // Determine if zone count is above threshold
    if(zone0Count > obstacleThreshold) zone0Clear = false;
    else zone0Clear = true;
    if(zone1Count > obstacleThreshold) zone1Clear = false;
    else zone1Clear = true;
    if(zone2Count > obstacleThreshold) zone2Clear = false;
    else zone2Clear = true;
    if(zone3Count > obstacleThreshold) zone3Clear = false;
    else zone3Clear = true;
    if(zone4Count > obstacleThreshold) zone4Clear = false;
    else zone4Clear = true;
    
    // zone0 - zone1 - zone2 - zone3 - zone4
    if(zone1Clear && zone2Clear && zone3Clear){
        strCmd = "STR090\n";
        moveCmd = "BWD080\n";
    }
    if(!zone0Clear && !zone1Clear && !zone2Clear && !zone3Clear && !zone4Clear){
        strCmd = "STR90\n";
        moveCmd = "BWD080\n";
    }
    if(!zone1Clear && !zone2Clear && !zone3Clear && zone0Clear){
        strCmd = "STR110\n";
        moveCmd = "BWD080\n";
    }
    if(!zone1Clear && !zone2Clear && !zone3Clear && zone4Clear){
        strCmd = "STR070\n";
        moveCmd = "BWD080\n";
    }
    if(!zone1Clear && !zone2Clear && zone3Clear){
        strCmd = "STR120\n";
        moveCmd = "FWD080\n";
    }
    if(zone1Clear && !zone2Clear && !zone3Clear){
        strCmd = "STR060\n";
        moveCmd = "FWD080\n";
    }
    if(!zone0Clear && zone1Clear){
        strCmd = "STR080\n";
        moveCmd = "FWD080";
    }
    if(!zone4Clear && zone3Clear){
        strCmd = "STR100\n";
        moveCmd = "FWD080\n";
    }
    
    // Write to serial port the driving commands
    printf("STR: %s, Move: %s\n", strCmd, moveCmd);
    bytesWritten = serialPortWrite(moveCmd,portID);
    bytesWritten = serialPortWrite(strCmd,portID);
}