#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

class PreprocessImage
{
    Mat inputImage;
    Mat outerBox;
    void preprocessImage(Mat &img)
    {
        inputImage = img;
    }

public:
    void preprocess()
    {
        outerBox = Mat(inputImage.size(), CV_8UC1);
    }
}
