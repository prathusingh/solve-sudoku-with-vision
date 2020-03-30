#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

class SudokuSolver
{
    void detectBoundingBox(Mat &outerBox)
    {
        int count = 0;
        int max = 1;
        Point maxPt;

        // find the seed point first

        for (int y = 0; y < outerBox.size().height; y++)
        {
            uchar *row = outerBox.ptr(y);
            for (int x = 0; x < outerBox.size().width; x++)
            {
                if (row[x] >= 128)
                {

                    int area = floodFill(outerBox, Point(x, y), CV_RGB(0, 0, 64));

                    if (area > max)
                    {
                        maxPt = Point(x, y);
                        max = area;
                    }
                }
            }
        }
        // flood with white color around the seed point that has maximum area
        floodFill(outerBox, maxPt, CV_RGB(255, 255, 255));

        // flood with blackmeans effectively hiding the grey points
        for (int y = 0; y < outerBox.size().height; y++)
        {
            uchar *row = outerBox.ptr(y);
            for (int x = 0; x < outerBox.size().width; x++)
            {
                if (row[x] == 64 && x != maxPt.x && y != maxPt.y)
                {
                    int area = floodFill(outerBox, Point(x, y), CV_RGB(0, 0, 0));
                }
            }
        }
    }

public:
    void preprocessImage(Mat &img)
    {
        // Create blank image of same size. it will hold the outerbox of same puzzle
        Mat outerBox = Mat(img.size(), CV_8UC1);
        // blur the image alittle. This smoothes out the noise and makes extracting the grid lines easier
        GaussianBlur(img, img, Size(11, 11), 0);

        adaptiveThreshold(img, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);

        // change the image from black to white
        bitwise_not(outerBox, outerBox);

        // dilating any small cracks in case Thresholding created some disconneccted region
        Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
        dilate(outerBox, outerBox, kernel);

        detectBoundingBox(outerBox);
        erode(outerBox, outerBox, kernel);

        // detect lines in bounding box by Hough Transformation
        std::vector<Vec2f> lines;
        HoughLines(outerBox, lines, 1, CV_PI / 180, 200);
        imshow("outerBox", outerBox);
    }
};

int main()
{
    std::cout << "Solve sudoku using vision" << std::endl;

    SudokuSolver sudokuSolver;
    // load the image in grayscale mode
    Mat sudoku = imread("../data/sudoku.jpg", 0);

    sudokuSolver.preprocessImage(sudoku);

    waitKey(0);
    return 0;
}