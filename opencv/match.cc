#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
    // Load the left and right images (replace with your image paths)
    Mat imgLeft = imread("leftBW.ppm");
    Mat imgRight = imread("rightBW.ppm");

    if (imgLeft.empty() || imgRight.empty()) {
        cout << "Error loading images" << endl;
        return -1;
    }

    // Stereo matching using StereoSGBM
    int minDisparity = 0;
    int numDisparities = 16 * 8; // Range of disparity
    int blockSize = 5;
    int speckleWindowSize = 100;
    int speckleRange = 32;
    int disp12MaxDiff = 1;
    int P1 = 8 * 3 * blockSize * blockSize;
    int P2 = 32 * 3 * blockSize * blockSize;

    Ptr<StereoSGBM> stereoSGBM = StereoSGBM::create(
        minDisparity, numDisparities, blockSize, P1, P2, disp12MaxDiff,
        speckleWindowSize, speckleRange, false
    );

    Mat disparityMap;
    stereoSGBM->compute(imgLeft, imgRight, disparityMap);

    // Convert disparity map to depth map (replace with your calibration values)
    double focalLength = 560; // Example focal length
    double baseline = 60.0; // Example baseline distance
    Mat depthMap;
    
    disparityMap.convertTo(depthMap, CV_32F);
    
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

    String outputFilename = "depth_map.png";
    bool saveSuccess = imwrite(outputFilename, depthMapNormalized);
    if (saveSuccess) {
        cout << "Depth map saved as " << outputFilename << endl;
    } else {
        cout << "Error: Could not save depth map image" << endl;
    }

    return 0;
}