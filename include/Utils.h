#ifndef Utils_H
#define Utils_H
#include <opencv2/opencv.hpp>

using namespace cv;

class Utils
{

public:
    static void drawLine(Vec2f line, Mat &img, Scalar rgb);
};

#endif
