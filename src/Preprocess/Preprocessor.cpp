#include <opencv2/opencv.hpp>
#include "Preprocessor.h"

using namespace cv;

void Preprocessor::preprocessImage(Mat img)
{
    Mat outerBox = Mat(img.size(), CV_8UC1);

    // smoothen out the noise
    GaussianBlur(img, img, Size(11, 11), 0);

    //threshold the image
    adaptiveThreshold(img, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);

    //convert borders to white from black since we are interested in them
    bitwise_not(outerBox, outerBox);

    // connect disconnected parts created during thresholding
    Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
    dilate(outerBox, outerBox, kernel);
}