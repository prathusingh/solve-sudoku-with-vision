#ifndef GridDetector_H
#define GridDetector_H
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class GridDetector
{
public:
    void findBiggestBlob(Mat box, vector<Vec2f> *lines);
    void mergeRelatedLines(vector<Vec2f> *lines, Mat img);
};

#endif