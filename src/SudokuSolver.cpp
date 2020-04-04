#include <iostream>
#include <opencv2/opencv.hpp>
#include "Preprocessor.h"

using namespace cv;

int main()
{
    std::cout << "Solve sudoku using vision" << std::endl;

    // load the image in grayscale mode
    Mat sudoku = imread("../data/sudoku.jpg", 0);

    Preprocessor preprocess;
    preprocess.preprocessImage(sudoku);

    waitKey(0);
    return 0;
}