#include <opencv2/opencv.hpp>
#include "GridDetector.h"

using namespace cv;
using namespace std;

void GridDetector::mergeRelatedLines(vector<Vec2f> *lines, Mat img)
{
    vector<Vec2f>::iterator current;
    for (current = lines->begin(); current != lines->end(); current++)
    {
        // line is already fused
        if ((*current)[0] == 0 && (*current)[1] == -100)
            continue;

        float ro1 = (*current)[0];
        float theta1 = (*current)[1];

        // find two points on the line
        Point pt1current, pt2current;

        // if line is horizontal: find point at the extremen left (x=0) and at extreme right (x=img.width)
        // if line is vertical find point at extreme top (y=0) and at extreme bottom (y=img.height)
        if (theta1 > CV_PI * 45 / 180 && theta1 < CV_PI * 135 / 180)
        {
            pt1current.x = 0;

            pt1current.y = ro1 / sin(theta1);

            pt2current.x = img.size().width;
            pt2current.y = -pt2current.x / tan(theta1) + ro1 / sin(theta1);
        }
        else
        {
            pt1current.y = 0;

            pt1current.x = ro1 / cos(theta1);

            pt2current.y = img.size().height;
            pt2current.x = -pt2current.y / tan(theta1) + ro1 / cos(theta1);
        }

        vector<Vec2f>::iterator pos;

        // compare every line with other line
        for (pos = lines->begin(); pos != lines->end(); pos++)
        {
            // same line
            if (*current == *pos)
                continue;
            // line is at a certain distance with current line
            if (fabs((*pos)[0] - (*current)[0]) < 20 && fabs((*pos)[1] - (*current)[1]) < CV_PI * 10 / 180)
            {
                // store row and theta for the line
                float row = (*pos)[0];
                float theta = (*pos)[1];

                // Again. find two points on the line
                Point pt1, pt2;
                if ((*pos)[1] > CV_PI * 45 / 180 && (*pos)[1] < CV_PI * 135 / 180)
                {
                    pt1.x = 0;
                    pt1.y = row / sin(theta);
                    pt2.x = img.size().width;
                    pt2.y = -pt2.x / tan(theta) + row / sin(theta);
                }
                else
                {
                    pt1.y = 0;
                    pt1.x = row / cos(theta);
                    pt2.y = img.size().height;
                    pt2.x = -pt2.y / tan(theta) + row / cos(theta);
                }
                // if endpoints of line pos and line current are close to each other, fuse them
                if (((double)(pt1.x - pt1current.x) * (pt1.x - pt1current.x) + (pt1.y - pt1current.y) * (pt1.y - pt1current.y) < 64 * 64) &&
                    ((double)(pt2.x - pt2current.x) * (pt2.x - pt2current.x) + (pt2.y - pt2current.y) * (pt2.y - pt2current.y) < 64 * 64))
                {
                    // Merge the two
                    (*current)[0] = ((*current)[0] + (*pos)[0]) / 2;

                    (*current)[1] = ((*current)[1] + (*pos)[1]) / 2;

                    // set imposiible params so that it is not fused again
                    (*pos)[0] = 0;
                    (*pos)[1] = -100;
                }
            }
        }
    }
}