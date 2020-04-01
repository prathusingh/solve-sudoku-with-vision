#ifndef Preprocessor_H
#define Preprocessor_H
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Preprocessor
{
public:
    void preprocessImage(Mat &img);
};

#endif