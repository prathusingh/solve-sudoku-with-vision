#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

class SudokuSolver
{
public:
    void preprocessImage(int val)
    {
        // blur the image alittle. This smoothes out the noise and makes extracting the grid lines easier
        std::cout << val << std::endl;
    }
};

int main()
{
    std::cout << "Solve sudoku using vision" << std::endl;
    // load the image in grayscale mode
    Mat sudoku = imread("../sudoku.jpg", 0);

    // Create blank image of same size. it will hold the outerbox of same puzzle
    Mat outerBox = Mat(sudoku.size(), CV_8UC1);

    preprocessImage(10);

    waitKey(0);
    return 0;
}