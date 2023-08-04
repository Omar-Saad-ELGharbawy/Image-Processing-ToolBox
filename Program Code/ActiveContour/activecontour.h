#ifndef ACTIVECONTOUR_H
#define ACTIVECONTOUR_H

#include "../Common.hpp"
#include <Filters/helper_functions.hpp>

double calcInternalEnergy(Point pt, Point prevPt, Point nextPt, double alpha);

double calcExternalEnergy(Mat img, Point pt, double beta);

double calcBalloonEnergy(Point pt, Point prevPt, double gamma);

double calcEnergy(Mat img, Point pt, Point prevPt, Point nextPt, double alpha, double beta, double gamma);

void moveCurve(Mat img, vector<Point>& curve, double alpha, double beta, double gamma);

void snake(Mat img, vector<Point>& curve, int numIterations, double alpha, double beta, double gamma);

vector<Point> initial_contour(Point center, int radius);

double points_distance(int x1, int y1, int x2, int y2);

double contour_area(vector<Point> points, int n_points);

double contour_perimeter(vector<Point> points, int n_points);

void display_area_perimeter(vector<Point> curve, Mat &outputImg);

vector<Point> active_Contour_Model(Mat img, Mat &outputImg, Point center, int radius, int numIterations, double alpha, double beta, double gamma);

// This defines an enum of the possible directions (or chain codes) that can be used to traverse a contour.
enum ChainCode
{
    East = 0,
    NorthEast = 1,
    North = 2,
    NorthWest = 3,
    West = 4,
    SouthWest = 5,
    South = 6,
    SouthEast = 7
};



Point getNeighbor(const Point &p, ChainCode direction);

int pixelValue(const std::vector<std::vector<int>> &image, const Point &p);

ChainCode findNextDirection(ChainCode currentDirection);


vector<ChainCode> chainCode(const std::vector<std::vector<int>> &image, const Point &startPoint);

Point findStartingPoint(const std::vector<std::vector<int>> &image);

vector<ChainCode> normalizeContour(const std::vector<ChainCode> &contour);

vector<std::vector<int>> createImageFromBoundary(const std::vector<Point> &boundary, int size);



#endif // ACTIVECONTOUR_H
