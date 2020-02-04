#include <iostream>
#include <opencv2/opencv.hpp>
//#include <highgui.h>

using namespace cv;

void drawLine(Vec2f line, Mat &img, Scalar rgb = CV_RGB(0, 0, 255))
{
    if (line[1] != 0)
    {
        float m = -1 / tan(line[1]);

        float c = line[0] / sin(line[1]);

        cv::line(img, Point(0, c), Point(img.size().width, m * img.size().width + c), rgb);
    }
    else
    {
        cv::line(img, Point(line[0], 0), Point(line[0], img.size().height), rgb);
    }
}

int main(int argc, char const *argv[])
{
    std::cout << "Solve sudoku using vision" << std::endl;
    Mat sudoku = imread("../sudoku.jpg", CV_8UC1);
    Mat outerBox = Mat(sudoku.size(), CV_8UC1);

    GaussianBlur(sudoku, sudoku, Size(45, 45), 0);
    imshow("image", sudoku);
    //adaptiveThreshold(sudoku, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);

    //bitwise_not(outerBox, outerBox);

    //Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
    //dilate(outerBox, outerBox, kernel);

    // find the biggest bounding box
    int count = 0;
    int max = -1;

    Point maxPt;

    // for (int y = 0; y < outerBox.size().height; y++)
    // {
    //     uchar *row = outerBox.ptr(y);
    //     for (int x = 0; x < outerBox.size().width; x++)
    //     {
    //         if (row[x] >= 128)
    //         {
    //             int area = floodFill(outerBox, Point(x, y), CV_RGB(0, 0, 64));
    //             if (area > max)
    //             {
    //                 maxPt = Point(x, y);
    //                 max = area;
    //             }
    //         }
    //     }
    // }

    // floodFill(outerBox, maxPt, CV_RGB(255, 255, 255));

    // for (int y = 0; y < outerBox.size().height; y++)
    // {
    //     uchar *row = outerBox.ptr(y);
    //     for (int x = 0; x < outerBox.size().width; x++)
    //     {
    //         if (row[x] == 64 && x != maxPt.x && y != maxPt.y)
    //         {
    //             int area = floodFill(outerBox, Point(x, y), CV_RGB(0, 0, 0));
    //         }
    //     }
    // }

    // erode(outerBox, outerBox, kernel);
    // imshow("thresholded", outerBox);

    // std::vector<Vec2f> lines;
    // HoughLines(outerBox, lines, 1, CV_PI / 180, 200);
    // for (int i = 0; i < lines.size(); i++)
    // {
    //     drawLine(lines[i], outerBox, CV_RGB(0, 0, 128));
    // }

    waitKey(0);
    return 0;
}
