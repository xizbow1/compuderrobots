#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
    // Load the left and right images
    Mat imgLeft = imread("leftBW.ppm", IMREAD_GRAYSCALE);
    Mat imgRight = imread("rightBW.ppm",IMREAD_GRAYSCALE);

    if (imgLeft.empty() || imgRight.empty()) {
        cout << "Error loading images" << endl;
        return -1;
    }

    // Stereo matching using StereoSGBM
    int minDisparity = 0;
    int numDisparities = 128; // Range of disparity
    int blockSize = 13;
    int speckleWindowSize = 200;
    int speckleRange = 32;
    int disp12MaxDiff = 1;
    int P1 = 8 * 1 * blockSize * blockSize;
    int P2 = 32 * 1 * blockSize * blockSize;

    Ptr<StereoSGBM> stereoSGBM = StereoSGBM::create(
        minDisparity, numDisparities, blockSize, P1, P2, disp12MaxDiff,
        speckleWindowSize, speckleRange, false
    );

    Mat leftGray, rightGray;
    if(imgLeft.channels() > 1){
        cvtColor(imgLeft, leftGray, COLOR_BGR2GRAY);
    } else {
        leftGray = imgLeft;
    }

    if(imgRight.channels() > 1){
        cvtColor(imgRight, rightGray, COLOR_BGR2GRAY);
    } else {
        rightGray = imgRight;
    }

    equalizeHist(leftGray, leftGray);
    equalizeHist(rightGray, rightGray);

    Mat disparityMap;
    stereoSGBM->compute(leftGray, rightGray, disparityMap);

    // Convert disparity map to depth map (replace with your calibration values)
    double focalLength = 560; // Example focal length
    double baseline = 60.0; // Example baseline distance
    Mat depthMap;
    
    disparityMap.convertTo(depthMap, CV_32F, 1.0/16.0);
    
    for (int y = 0; y < depthMap.rows; y++) {
        for (int x = 0; x < depthMap.cols; x++) {
            float disparity = depthMap.at<float>(y, x);
            if (disparity > 0) {
                depthMap.at<float>(y, x) = (focalLength * baseline) / disparity;
            } else {
                depthMap.at<float>(y, x) = 0; // Or a large value to indicate invalid depth
            }
        }
    }
    
    // Normalize and display depth map
    Mat depthMapNormalized;
    normalize(depthMap, depthMapNormalized, 0, 255, NORM_MINMAX, CV_8U);

    bitwise_not(depthMapNormalized, depthMapNormalized);

    String outputFilename = "depth_map.png";
    bool saveSuccess = imwrite(outputFilename, depthMapNormalized);
    if (saveSuccess) {
        cout << "Depth map saved as " << outputFilename << endl;
    } else {
        cout << "Error: Could not save depth map image" << endl;
    }

    return 0;
}