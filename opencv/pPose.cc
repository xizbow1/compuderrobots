#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Prepare object points (3D points in real world space)
    int board_width = 9;  // Checkerboard width (columns)
    int board_height = 6; // Checkerboard height (rows)
    float square_size = 23.0f; // The size of a square mm 
    int imageWidth  = 640;
    int imageHeight = 480;

    std::vector<cv::Point3f> objectPoints; // 3D points in real world space
    for (int i = 0; i < board_height; i++) {
        for (int j = 0; j < board_width; j++) {
            objectPoints.push_back(cv::Point3f(j * square_size, i * square_size, 0.0f));
        }
    }

    // Vector to store the 2D image points and object points from each image
    std::vector<std::vector<cv::Point2f>> imagePointsLeft,imagePointsRight; // 2D points in the image plane
    std::vector<std::vector<cv::Point3f>> objectPointsAll; // 3D object points
    std::vector<cv::Mat> rvecs, tvecs; // Rotation and translation vectors

    printf("read images on line 26 \n");
    // Read images for calibration
    std::vector<cv::String> fileNamesLeft;
    std::vector<cv::String> fileNamesRight;
    cv::glob("nvcamtest_7529_s00_*.jpg", fileNamesLeft);
    cv::glob("nvcamtest_7559_s01_*.jpg", fileNamesRight); 

    printf("find image  points on line 33 \n");
// find  image points
    for (size_t i = 0; i < fileNamesLeft.size(); i++) {
   
         cv::Mat leftImage = cv::imread(fileNamesLeft[i]);
         cv::Mat rightImage = cv::imread(fileNamesRight[i]);
 
         cv::Mat grayLeft,grayRight;
         cv::cvtColor(leftImage, grayLeft, cv::COLOR_BGR2GRAY);
         cv::cvtColor(rightImage, grayRight, cv::COLOR_BGR2GRAY);
 
        vector<Point2f> cornersL, cornersR;
        bool foundL = findChessboardCorners(grayLeft, Size(board_width, board_height), cornersL);
        bool foundR = findChessboardCorners(grayRight, Size(board_width, board_height), cornersR);

        if (foundL && foundR) {
            // Refine corner positions
            cornerSubPix(grayLeft, cornersL, Size(11, 11), Size(-1, -1),
                         TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.01));
            cornerSubPix(grayRight, cornersR, Size(11, 11), Size(-1, -1),
                         TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.01));

            // Store the detected points
            objectPointsAll.push_back(objectPoints);
            imagePointsLeft.push_back(cornersL);
            imagePointsRight.push_back(cornersR);

 // Draw corners for visualization
             cv::drawChessboardCorners(grayLeft, cv::Size(board_width, board_height), cornersL, foundL);
             cv::imshow("Chessboard", grayLeft);
             cv::waitKey(500); // Wait for half a second before moving to the next image
             cv::drawChessboardCorners(grayRight, cv::Size(board_width, board_height), cornersR, foundR);
             cv::imshow("Chessboard", grayRight);
             cv::waitKey(1000); // Wait for half a second before moving to the next image

        }
    }


    cv::destroyAllWindows();

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
  

   printf("calibrate camera on line 96 \n");
    // Perform the calibration
    Mat R, T, E, F;
    Size imageSize(imageWidth,imageHeight) ;

    double rms = stereoCalibrate(objectPointsAll, imagePointsLeft, imagePointsRight,
                                 cameraMatrix1, distCoeffs1,
                                 cameraMatrix2, distCoeffs2,
                                 imageSize, R, T, E, F,
                                 CALIB_USE_INTRINSIC_GUESS,
                                 TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 100, 1e-5));
  
    cout << "Stereo Calibration RMS error: " << rms << endl;
    cout << "Rotation matrix R:\n" << R << endl;
    cout << "Translation vector T:\n" << T << endl;


    return 0;
}

