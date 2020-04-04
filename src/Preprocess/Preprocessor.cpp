#include <opencv2/opencv.hpp>
#include "Preprocessor.h"

using namespace cv;

void Preprocessor::preprocessImage(Mat img)
{
    imshow("image", img);
}