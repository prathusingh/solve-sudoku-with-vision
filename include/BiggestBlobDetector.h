#ifndef BiggestBlobDetector_H
#define BiggestBlobDetector_H
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class BiggestBlobDetector
{
public:
    void findBiggestBlob(Mat box, vector<Vec2f> *lines);
};

#endif