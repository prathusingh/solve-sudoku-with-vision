#ifndef GridDetector_H
#define GridDetector_H
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class GridDetector
{
public:
    void mergeRelatedLines(vector<Vec2f> *lines, Mat img);
    void findExtremeLines(vector<Vec2f> *lines, Mat img);
};

#endif