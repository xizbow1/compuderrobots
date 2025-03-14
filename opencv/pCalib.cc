#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

int main() {
    // Prepare object points (3D points in real world space)
    int board_width = 9;  // Checkerboard width (columns)
    int board_height = 6; // Checkerboard height (rows)
    float square_size = 23.0f; // The size of a square mm 
    int imageWidth =640;
    int imageHeight =480;

    std::vector<cv::Point3f> object_points; // 3D points in real world space
    for (int i = 0; i < board_height; i++) {
        for (int j = 0; j < board_width; j++) {
            object_points.push_back(cv::Point3f(j * square_size, i * square_size, 0.0f));
        }
    }

    // Vector to store the 2D image points and object points from each image
    std::vector<std::vector<cv::Point2f>> image_points; // 2D points in the image plane
    std::vector<std::vector<cv::Point3f>> object_points_all; // 3D object points
    std::vector<cv::Mat> rvecs, tvecs; // Rotation and translation vectors

    // Read images for calibration
    std::vector<cv::String> filenames;
//    cv::glob("nvcamtest_7559_s01_*.jpg", filenames); 
  cv::glob("nvcamtest_7529_s00_*.jpg", filenames); 

    for (size_t i = 0; i < filenames.size(); i++) {
        cv::Mat img = cv::imread(filenames[i]);
        cv::Mat gray;
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Point2f> corners;

        // Find the chessboard corners
        bool found = cv::findChessboardCorners(gray, cv::Size(board_width, board_height), corners, 
                                                cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

        if (found) {

            // Refine corner locations
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), 
                          cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.1));
            
            // store corners and image points
             image_points.push_back(corners);
             object_points_all.push_back(object_points);

            // Draw corners for visualization
            cv::drawChessboardCorners(img, cv::Size(board_width, board_height), corners, found);
            cv::imshow("Chessboard", img);
            cv::waitKey(1000); // Wait for half a second before moving to the next image
        }
    }

    cv::destroyAllWindows();

    // Calibrate the camera
    cv::Mat camera_matrix = cv::Mat::eye(3, 3, CV_64F);  // Intrinsic camera matrix
    cv::Mat dist_coeffs = cv::Mat::zeros(8, 1, CV_64F);  // Distortion coefficients

    // Perform the calibration
    double rms = cv::calibrateCamera(object_points_all, image_points, cv::Size(imageWidth,imageHeight), 
                                     camera_matrix, dist_coeffs, rvecs, tvecs);

    std::cout << "Camera calibration done with RMS error: " << rms << std::endl;

    std::cout << "Camera Matrix: " << camera_matrix << std::endl;
    std::cout << "Distortion Coefficients: " << dist_coeffs << std::endl;


    return 0;
}

