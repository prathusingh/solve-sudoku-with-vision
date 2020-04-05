#include <opencv2/opencv.hpp>
#include "Shared.h"
#include "GridDetector.h"

using namespace cv;

void GridDetector::findBiggestBlob(Mat box, std::vector<Vec2f> lines)
{
    int count = 0;
    int max = -1;

    Point maxPt;

    // find the bounding box area
    for (int y = 0; y < box.size().height; y++)
    {
        uchar *row = box.ptr(y);
        for (int x = 0; x < box.size().width; x++)
        {
            if (row[x] >= 128)
            {

                int area = floodFill(box, Point(x, y), CV_RGB(0, 0, 64));

                if (area > max)
                {
                    maxPt = Point(x, y);
                    max = area;
                }
            }
        }
    }

    // flood fill the outer box
    floodFill(box, maxPt, CV_RGB(255, 255, 255));

    // Turn other blobs black, since bounding box is captured
    for (int y = 0; y < box.size().height; y++)
    {
        uchar *row = box.ptr(y);
        for (int x = 0; x < box.size().width; x++)
        {
            if (row[x] == 64 && x != maxPt.x && y != maxPt.y)
            {
                int area = floodFill(box, Point(x, y), CV_RGB(0, 0, 0));
            }
        }
    }
    erode(box, box, kernel);

    HoughLines(box, lines, 1, CV_PI / 180, 200);
}
