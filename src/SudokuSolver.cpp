#include <iostream>
#include <opencv2/opencv.hpp>
#include "Shared.h"
#include "Preprocessor.h"
#include "GridDetector.h"
#include "BiggestBlobDetector.h"

using namespace cv;

Mat kernel;

int main()
{
    std::cout << "Solve Sudoku using Computer Vision" << std::endl;

    // load the image in grayscale mode
    Mat sudoku = imread("../src/data/sudoku.jpg", 0);

    kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);

    // preprocess the image
    Preprocessor preprocess;
    Mat outerBox = preprocess.preprocessImage(sudoku);

    // detect biggest blob (bounding box)
    std::vector<Vec2f> lines;
    BiggestBlobDetector blobDetector;
    blobDetector.findBiggestBlob(outerBox, &lines);

    // extract grid
    GridDetector gridDetector;
    gridDetector.mergeRelatedLines(&lines, sudoku);
    std::map<string, Vec2f> edges = gridDetector.findExtremeLines(&lines, sudoku);
    gridDetector.calculateIntersection(edges, sudoku);

    return 0;
}