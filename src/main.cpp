#include <iostream>
#include <opencv2/opencv.hpp>
//#include <highgui.h>

using namespace cv;

int main(int argc, char const *argv[])
{
    std::cout << "Solve sudoku using vision" << std::endl;
    Mat sudoku = imread("../sudoku.jpg", 1);
    GaussianBlur(sudoku, sudoku, Size(11, 11), 0);
    imshow("Sudoku", sudoku);
    waitKey(0);
    return 0;
}
