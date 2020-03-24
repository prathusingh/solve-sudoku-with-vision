#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

class SudokuSolver
{
public:
    void preprocessImage(Mat &img)
    {
        // Create blank image of same size. it will hold the outerbox of same puzzle
        Mat outerBox = Mat(img.size(), CV_8UC1);
        // blur the image alittle. This smoothes out the noise and makes extracting the grid lines easier
        GaussianBlur(img, img, Size(11, 11), 0);
        imshow("sudoku", img);
    }
};

int main()
{
    std::cout << "Solve sudoku using vision" << std::endl;

    SudokuSolver sudokuSolver;
    // load the image in grayscale mode
    Mat sudoku = imread("../sudoku.jpg", 0);

    sudokuSolver.preprocessImage(sudoku);

    waitKey(0);
    return 0;
}