#include <opencv2/opencv.hpp>
#include "GridDetector.h"
#include "Utils.h"

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

        float rho1 = (*current)[0];
        float theta1 = (*current)[1];

        // find two points on the line
        Point pt1current, pt2current;

        // if line is horizontal: find point at the extremen left (x=0) and at extreme right (x=img.width)
        // if line is vertical find point at extreme top (y=0) and at extreme bottom (y=img.height)
        if (theta1 > CV_PI * 45 / 180 && theta1 < CV_PI * 135 / 180)
        {
            pt1current.x = 0;

            pt1current.y = rho1 / sin(theta1);

            pt2current.x = img.size().width;
            pt2current.y = -pt2current.x / tan(theta1) + rho1 / sin(theta1);
        }
        else
        {
            pt1current.y = 0;

            pt1current.x = rho1 / cos(theta1);

            pt2current.y = img.size().height;
            pt2current.x = -pt2current.y / tan(theta1) + rho1 / cos(theta1);
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
                // store rho and theta for the line
                float rho = (*pos)[0];
                float theta = (*pos)[1];

                // Again. find two points on the line
                Point pt1, pt2;
                if ((*pos)[1] > CV_PI * 45 / 180 && (*pos)[1] < CV_PI * 135 / 180)
                {
                    pt1.x = 0;
                    pt1.y = rho / sin(theta);
                    pt2.x = img.size().width;
                    pt2.y = -pt2.x / tan(theta) + rho / sin(theta);
                }
                else
                {
                    pt1.y = 0;
                    pt1.x = rho / cos(theta);
                    pt2.y = img.size().height;
                    pt2.x = -pt2.y / tan(theta) + rho / cos(theta);
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

map<string, Vec2f> GridDetector ::findExtremeLines(vector<Vec2f> *lines, Mat img)
{
    // Now detect the lines on extremes
    // initial value of edge is some ridiculous value
    Vec2f topEdge = Vec2f(1000, 1000);
    double topYIntercept = 100000, topXIntercept = 0;
    Vec2f bottomEdge = Vec2f(-1000, -1000);
    double bottomYIntercept = 0, bottomXIntercept = 0;
    Vec2f leftEdge = Vec2f(1000, 1000);
    double leftXIntercept = 100000, leftYIntercept = 0;
    Vec2f rightEdge = Vec2f(-1000, -1000);
    double rightXIntercept = 0, rightYIntercept = 0;

    map<string, Vec2f> extremeLines;
    vector<Vec2f>::iterator current;
    pair<string, Vec2f> edgesPair;

    for (current = lines->begin(); current != lines->end(); current++)
    {
        float rho = (*current)[0];

        float theta = (*current)[1];

        if (rho == 0 && theta == -100)
            continue;

        double xIntercept, yIntercept;
        xIntercept = rho / cos(theta);
        yIntercept = rho / (cos(theta) * sin(theta));

        // if the line is nearly vertical
        if (theta > CV_PI * 80 / 180 && theta < CV_PI * 100 / 180)
        {
            if (rho < topEdge[0])
            {
                topEdge = *current;
                pair<string, Vec2f> topEdgePair("topEdge", topEdge);
                extremeLines.insert(topEdgePair);
            }

            if (rho > bottomEdge[0])
            {
                bottomEdge = *current;
                pair<string, Vec2f> bottomEdgePair("bottomEdge", bottomEdge);
                extremeLines.insert(bottomEdgePair);
            }
        } // if the line is nearly horizontal
        else if (theta < CV_PI * 10 / 180 || theta > CV_PI * 170 / 180)
        {
            if (xIntercept > rightXIntercept)
            {
                rightEdge = *current;
                pair<string, Vec2f> rightEdgePair("rightEdge", rightEdge);
                extremeLines.insert(rightEdgePair);
                rightXIntercept = xIntercept;
            }
            else if (xIntercept <= leftXIntercept)
            {
                leftEdge = *current;
                pair<string, Vec2f> leftEdgePair("leftEdge", leftEdge);
                extremeLines.insert(leftEdgePair);
                leftXIntercept = xIntercept;
            }
        }
    }

    Utils::drawLine(topEdge, img, CV_RGB(0, 0, 0));
    Utils::drawLine(bottomEdge, img, CV_RGB(0, 0, 0));
    Utils::drawLine(leftEdge, img, CV_RGB(0, 0, 0));
    Utils::drawLine(rightEdge, img, CV_RGB(0, 0, 0));

    return extremeLines;
}

void GridDetector::calculateIntersection(map<string, Vec2f> edges, Mat outerBox)
{
    // find two points on each line
    Point left1, left2, right1, right2, bottom1, bottom2, top1, top2;

    int height = outerBox.size().height;

    int width = outerBox.size().width;

    if (edges["leftEdge"][1] != 0)
    {
        left1.x = 0;
        left1.y = edges["leftEdge"][0] / sin(edges["leftEdge"][1]);
        left2.x = width;
        left2.y = -left2.x / tan(edges["leftEdge"][1]) + left1.y;
    }
    else
    {
        left1.y = 0;
        left1.x = edges["leftEdge"][0] / cos(edges["leftEdge"][1]);
        left2.y = height;
        left2.x = left1.x - height * tan(edges["leftEdge"][1]);
    }

    if (edges["rightEdge"][1] != 0)
    {
        right1.x = 0;
        right1.y = edges["rightEdge"][0] / sin(edges["rightEdge"][1]);
        right2.x = width;
        right2.y = -right2.x / tan(edges["rightEdge"][1]) + right1.y;
    }
    else
    {
        right1.y = 0;
        right1.x = edges["rightEdge"][0] / cos(edges["rightEdge"][1]);
        right2.y = height;
        right2.x = right1.x - height * tan(edges["rightEdge"][1]);
    }

    bottom1.x = 0;
    bottom1.y = edges["bottomEdge"][0] / sin(edges["bottomEdge"][1]);

    bottom2.x = width;
    bottom2.y = -bottom2.x / tan(edges["bottomEdge"][1]) + bottom1.y;

    top1.x = 0;
    top1.y = edges["topEdge"][0] / sin(edges["topEdge"][1]);
    top2.x = width;
    top2.y = -top2.x / tan(edges["topEdge"][1]) + top1.y;

    // Next, we find the intersection of  these four lines
    double leftA = left2.y - left1.y;
    double leftB = left1.x - left2.x;

    double leftC = leftA * left1.x + leftB * left1.y;

    double rightA = right2.y - right1.y;
    double rightB = right1.x - right2.x;

    double rightC = rightA * right1.x + rightB * right1.y;

    double topA = top2.y - top1.y;
    double topB = top1.x - top2.x;

    double topC = topA * top1.x + topB * top1.y;

    double bottomA = bottom2.y - bottom1.y;
    double bottomB = bottom1.x - bottom2.x;

    double bottomC = bottomA * bottom1.x + bottomB * bottom1.y;

    // Intersection of left and top
    double detTopLeft = leftA * topB - leftB * topA;

    Point ptTopLeft = cv::Point((topB * leftC - leftB * topC) / detTopLeft, (leftA * topC - topA * leftC) / detTopLeft);

    // Intersection of top and right
    double detTopRight = rightA * topB - rightB * topA;

    Point ptTopRight = cv::Point((topB * rightC - rightB * topC) / detTopRight, (rightA * topC - topA * rightC) / detTopRight);

    // Intersection of right and bottom
    double detBottomRight = rightA * bottomB - rightB * bottomA;
    Point ptBottomRight = cv::Point((bottomB * rightC - rightB * bottomC) / detBottomRight, (rightA * bottomC - bottomA * rightC) / detBottomRight); // Intersection of bottom and left
    double detBottomLeft = leftA * bottomB - leftB * bottomA;
    Point ptBottomLeft = cv::Point((bottomB * leftC - leftB * bottomC) / detBottomLeft, (leftA * bottomC - bottomA * leftC) / detBottomLeft);

    // find the longest edge of the puzzle
    int maxLength = (ptBottomLeft.x - ptBottomRight.x) * (ptBottomLeft.x - ptBottomRight.x) + (ptBottomLeft.y - ptBottomRight.y) * (ptBottomLeft.y - ptBottomRight.y);
    int temp = (ptTopRight.x - ptBottomRight.x) * (ptTopRight.x - ptBottomRight.x) + (ptTopRight.y - ptBottomRight.y) * (ptTopRight.y - ptBottomRight.y);

    if (temp > maxLength)
        maxLength = temp;

    temp = (ptTopRight.x - ptTopLeft.x) * (ptTopRight.x - ptTopLeft.x) + (ptTopRight.y - ptTopLeft.y) * (ptTopRight.y - ptTopLeft.y);

    if (temp > maxLength)
        maxLength = temp;

    temp = (ptBottomLeft.x - ptTopLeft.x) * (ptBottomLeft.x - ptTopLeft.x) + (ptBottomLeft.y - ptTopLeft.y) * (ptBottomLeft.y - ptTopLeft.y);

    if (temp > maxLength)
        maxLength = temp;

    maxLength = sqrt((double)maxLength);

    Point2f src[4], dst[4];
    src[0] = ptTopLeft;
    dst[0] = Point2f(0, 0);
    src[1] = ptTopRight;
    dst[1] = Point2f(maxLength - 1, 0);
    src[2] = ptBottomRight;
    dst[2] = Point2f(maxLength - 1, maxLength - 1);
    src[3] = ptBottomLeft;
    dst[3] = Point2f(0, maxLength - 1);

    Mat undistorted = Mat(Size(maxLength, maxLength), CV_8UC1);
    cv::warpPerspective(outerBox, undistorted, cv::getPerspectiveTransform(src, dst), Size(maxLength, maxLength));

    imshow("undistorted", undistorted);
    waitKey(7000); // 3 seconds
}