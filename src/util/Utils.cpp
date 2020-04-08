#include <iostream>
#include <opencv2/opencv.hpp>
#include "Utils.h"

using namespace cv;

void Utils::drawLine(Vec2f line, Mat &img, Scalar rgb = CV_RGB(0, 0, 255))
{
    if (line[1] != 0)
    {
        float m = -1 / tan(line[1]);

        float c = line[0] / sin(line[1]);

        cv::line(img, Point(0, c), Point(img.size().width, m * img.size().width + c), rgb);
    }
    else
    {
        cv::line(img, Point(line[0], 0), Point(line[0], img.size().height), rgb);
    }
}
