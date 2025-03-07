#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {

vector<Point3f> objectPoints;
vector<Point2f> imagePoints;
Mat cameraMatrix;
int rightCamera;

// read the camera calibration parameters 
cout << "Enter Camera. 0=left or 1=right) ";
cin >> rightCamera;


if(rightCamera){

//right camera
imagePoints ={ 
              {259, 478},
              {284, 466},
              {310, 454},
              {331, 444},
              {353, 434},
              {309, 407},
              {311, 362},
              {231, 467},
              {209, 456},
              {186, 447},
              {168, 437},
              {208, 409},
              {206, 361},
              {260, 426},
              {258, 375},
              {260, 349},
              {260, 329},
              {166, 349},
              {350, 348},
              {304, 338}};

cameraMatrix = (Mat_<double>(3, 3) << 622, 0, 256, 0, 766, 273, 0, 0, 1);

}else{

//Left camera 
imagePoints ={ 
                {387, 471},
                {409, 458},
                {429, 446},
                {447, 434},
                {463, 427},
                {430, 398},
                {430, 351},
                {355, 461},
                {327, 450},
                {301, 438},
                {278, 431},
                {327, 400},
                {327, 353},
                {389, 418},
                {387, 367},
                {371, 339},
                {363, 321},
                {277, 341},
                {462, 336},
                {408, 328}};

cameraMatrix = (Mat_<double>(3, 3) << 506, 0, 260 , 0, 625, 227 , 0, 0, 1);

}

//World coordinate points x=left/right, y=up, z=front/back (x,y,z)
//looking down on the cube's corner nearest the camera
//                 z
//                 |
//                 |
//                 |
//   x_____________|            
//                 y(up)  

objectPoints={
               {   0.0, 0.0,   0.0},
               {-17.68, 0.0,  18.0},
               {-35.36, 0.0,  35.0},
               {-53.03, 0.0,  53.0},
               {-70.71, 0.0,  66.0},
               {-35.36, 25.0, 35.0},
               {-35.36, 50.0, 35.0},
               { 17.68, 0.0,  18.0},
               { 35.36, 0.0,  35.0},
               { 50.03, 0.0,  53.0},
               { 70.01, 0.0,  68.0},
               { 35.36, 25.0, 35.0},
               { 35.36, 50.0, 35.0},
               {   0.0, 25.0, 0.0},
               {   0.0, 50.0, 0.0},
               {   0.0, 50.0, 68.0},
               {   0.0, 50.0, 136.0},
               { 70.71, 50.0, 68.0},
               {-70.71, 50.0, 68.0},
               { -1.41, 50.0, 103.0}};

    // Distortion coefficients (assuming no distortion)
     Mat distCoeffs = Mat::zeros(4, 1, CV_64F);

    // Rotation and translation vectors
    Mat rvec, tvec;

    // Solve PnP to estimate pose
    bool success = solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);

    if (success) {
        cout << "Translation Vector: \n" << tvec << endl;

        // Convert rotation vector to rotation matrix
        Mat R;
        Rodrigues(rvec, R);

        cout << "Rotation Matrix: \n" << R << endl;
    } else {
        cerr << "PnP solution failed!" << endl;
    }

    return 0;
}

