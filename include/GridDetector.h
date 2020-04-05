#ifndef GridDetector_H
#define GridDetector_H
#include <opencv2/opencv.hpp>

using namespace cv;

class GridDetector
{
public:
    void findBiggestBlob(Mat box, std::vector<Vec2f> lines);
};

#endif