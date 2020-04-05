#include <iostream>
#include <opencv2/opencv.hpp>
#include "Shared.h"
#include "Preprocessor.h"
#include "GridDetector.h"

using namespace cv;

Mat kernel;

int main()
{
    std::cout << "Solve sudoku using vision" << std::endl;

    // load the image in grayscale mode
    Mat sudoku = imread("../src/data/sudoku.jpg", 0);

    kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);

    // preprocess the image
    Preprocessor preprocess;
    Mat outerBox = preprocess.preprocessImage(sudoku);

    // extract grid
    GridDetector gridDetector;
    gridDetector.findBiggestBlob(outerBox);

    imshow("thresholded", outerBox);
    waitKey(0);
    return 0;
}