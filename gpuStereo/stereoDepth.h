#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
 
 using namespace cv;
 using namespace std;
 
void stereoDepth(Mat* left, Mat* right, Mat* depth, double maxDistance, int rows, int cols); 

