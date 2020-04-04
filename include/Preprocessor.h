#ifndef Preprocessor_H
#define Preprocessor_H
#include <opencv2/opencv.hpp>

using namespace cv;

class Preprocessor
{
public:
    Mat preprocessImage(Mat img);
};

#endif