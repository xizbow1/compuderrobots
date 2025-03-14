#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    
// Load left and right stereo images
    Mat leftImageColor  = imread("nvcamtest_6735_s00_00000.jpg");
    Mat rightImageColor = imread("nvcamtest_6657_s01_00000.jpg");

     cv::Mat leftImage,rightImage;
     cv::cvtColor(leftImageColor,leftImage, cv::COLOR_BGR2GRAY);
     cv::cvtColor(rightImageColor,rightImage, cv::COLOR_BGR2GRAY);

    imshow("left",leftImage);
    imshow("right",rightImage);

    if (leftImage.empty() || rightImage.empty()) {
        cout << "Error: Could not load stereo images!" << endl;
        return -1;
    }

 
// Camera parameters (intrinsic matrices)
       Mat cameraMatrix1 = (Mat_<double>(3,3) << 578, 0, 330,
                                                 0, 773, 245,
                                                  0,0,1);
        Mat cameraMatrix2 = (Mat_<double>(3,3) << 575, 0, 303,
                                                  0, 768, 251,
                                                  0 ,0, 1);
  
        // Distortion coefficients
        Mat distCoeffs1 = (Mat_<double>(1, 5) << -0.168,1.17,0.00,0.00,-2.57);
        Mat distCoeffs2 = (Mat_<double>(1, 5) << -0.178,1.45,0.00,0.00,-3.73);

//Rotation matrix R:
 Mat R = (Mat_<double>(3,3) <<0.9998996653444937, -0.007885144718599264, -0.01176791131579057,
                              0.007832711413248565, 0.9999592206338627, -0.004495075220148332,
                              0.0118028757464582, 0.004402449555051375, 0.9999206521329724);
//Translation vector T:
Mat T = (Mat_<double>(3,1) << -58.96213584809675,
                              -0.3678605528979815,
                              -0.6298580023378643);
// Output rectification transforms, projection matrices, and disparity-to-depth mapping matrix
    Mat R1, R2, P1, P2, Q;
    
    // Compute rectification transforms
    stereoRectify(cameraMatrix1, distCoeffs1, cameraMatrix2, distCoeffs2, leftImage.size(), 
                  R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY,-1,leftImage.size());
    
    // Compute undistortion and rectification maps
    Mat map1x, map1y, map2x, map2y;
    initUndistortRectifyMap(cameraMatrix1, distCoeffs1, R1, P1, leftImage.size(), CV_32FC1, map1x, map1y);
    initUndistortRectifyMap(cameraMatrix2, distCoeffs2, R2, P2, rightImage.size(), CV_32FC1, map2x, map2y);
    
    // Apply rectification
    Mat rectifiedLeft, rectifiedRight;
    remap(leftImage, rectifiedLeft, map1x, map1y, INTER_LINEAR);
    remap(rightImage, rectifiedRight, map2x, map2y, INTER_LINEAR);
    
    // Display results
    imshow("Rectified Left Image", rectifiedLeft);
    imshow("Rectified Right Image", rectifiedRight);

   // Display original images
    imwrite("leftRectified.jpg", rectifiedLeft); 
    imwrite("rightRectified.jpg", rectifiedRight); 
    waitKey(0);
    return 0;
}

