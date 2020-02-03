#include <iostream>
#include <opencv2/opencv.hpp>
//#include <highgui.h>

using namespace cv;

int main(int argc, char const *argv[])
{
    std::cout << "Solve sudoku using vision" << std::endl;
    Mat sudoku = imread("../sudoku.jpg", CV_8UC1);
    Mat outerBox = Mat(sudoku.size(), CV_8UC1);

    GaussianBlur(sudoku, sudoku, Size(11, 11), 0);
    adaptiveThreshold(sudoku, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
    bitwise_not(outerBox, outerBox);
    Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
    dilate(outerBox, outerBox, kernel);
    //imshow("Sudoku", sudoku);
    imshow("Outer box", outerBox);
    waitKey(0);
    return 0;
}
