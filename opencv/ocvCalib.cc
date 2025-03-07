#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
int rightCamera;

 // read the camera calibration parameters
 cout << "Enter Camera. 0=left or 1=right) ";
 cin >> rightCamera;

    
vector<Point3f> object_points_single;
vector<Point2f> image_points_single;

if(!rightCamera){
//Left camera 
image_points_single ={
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

}else{

image_points_single ={ 
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
}


 //World coordinate points x=left/right, y=up, z=front/back (x,y    ,z)
 //looking down on the cube's corner nearest the camera
 //                 z
 //                 |
 //                 |
 //                 |
 //   x_____________|
 //                 y(up)

 object_points_single={
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
vector<vector<Point3f>> object_points;  // vector of vectors for object points
vector<vector<Point2f>> image_points;   // vector of vectors for image points

 object_points.push_back(object_points_single);  
 image_points.push_back(image_points_single);  

// Camera calibration: find intrinsic parameters and distortion coefficients
    Mat camera_matrix = (Mat_<double>(3, 3) << 563, 0, 265 , 0, 745, 211 , 0, 0, 1);
    Mat dist_coeffs = Mat::zeros(8, 1, CV_64F);  // assuming 5 distortion coefficients
    vector<Mat> rvecs, tvecs;

    double rms = calibrateCamera(object_points, image_points, Size(640, 480), camera_matrix, dist_coeffs, rvecs, tvecs,CALIB_USE_INTRINSIC_GUESS);

    cout << "RMS error: " << rms << endl;
    cout << "Camera Matrix: " << camera_matrix << endl;
    cout << "Distortion Coefficients: " << dist_coeffs << endl;

    // Save the camera matrix and distortion coefficients
    FileStorage fs("camera_params.xml", FileStorage::WRITE);
    fs << "camera_matrix" << camera_matrix;
    fs << "distortion_coefficients" << dist_coeffs;
    fs.release();

    waitKey(0);
    return 0;
}

