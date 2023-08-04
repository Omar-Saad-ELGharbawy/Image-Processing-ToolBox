#include "activecontour.h"


// Function to calculate the internal energy term
double calcInternalEnergy(Point pt, Point prevPt, Point nextPt, double alpha) {
    double dx1 = pt.x - prevPt.x;
    double dy1 = pt.y - prevPt.y;
    double dx2 = nextPt.x - pt.x;
    double dy2 = nextPt.y - pt.y;
    double curvature = (dx1 * dy2 - dx2 * dy1) / pow(dx1*dx1 + dy1*dy1, 1.5);
    return alpha * curvature;
}

// Function to calculate the external energy term
double calcExternalEnergy(Mat img, Point pt, double beta) {
    return -beta * img.at<uchar>(pt.y, pt.x);
}

// Function to calculate the balloon energy term
double calcBalloonEnergy(Point pt, Point prevPt, double gamma) {
    double dx = pt.x - prevPt.x;
    double dy = pt.y - prevPt.y;
    return gamma * (dx*dx + dy*dy);
}

// Function to calculate the energy at a point on the curve
double calcEnergy(Mat img, Point pt, Point prevPt, Point nextPt, double alpha, double beta, double gamma) {
    double Einternal = calcInternalEnergy(pt, prevPt, nextPt, alpha);
    double Eexternal = calcExternalEnergy(img, pt, beta);
    double Eballoon = calcBalloonEnergy(pt, prevPt, gamma);
    return Einternal + Eexternal + Eballoon;
}

// Function to move the curve to minimize the energy function
void moveCurve(Mat img, vector<Point>& curve, double alpha, double beta, double gamma) {
    int numPoints = curve.size();  // example --> 10 points
    vector<Point> newCurve(numPoints);
    for (int i = 0; i < numPoints; i++) {
        //  5 6 7 8 9 0 1 2 3 4
        Point pt = curve[i];
        Point prevPt = curve[(i-1+numPoints)%numPoints];
        Point nextPt = curve[(i+1)%numPoints];
        double minEnergy = DBL_MAX; // max value can a double variable hold
        Point newPt = pt;
        // Try moving the point in different directions and choose the one with the minimum energy
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                Point movePt(pt.x + dx, pt.y + dy);
                double energy = calcEnergy(img, movePt, prevPt, nextPt, alpha, beta, gamma);
                if (energy < minEnergy) {
                    minEnergy = energy;
                    newPt = movePt;
                }
            }
        }
        newCurve[i] = newPt;
    }
    curve = newCurve;
}

// Function to run the Snake algorithm for a fixed number of iterations
void snake(Mat img, vector<Point>& curve, int numIterations, double alpha, double beta, double gamma) {
    // Convert image to grayscale
    Mat grayImg;
    Convert_To_Gray(img, grayImg);
    blur(grayImg, grayImg, Size(5,5));

    // Iterate for multiple iterations
    for (int i = 0; i < numIterations; i++) {
        moveCurve(grayImg, curve, alpha, beta, gamma);
    }
}

vector<Point> initial_contour(Point center, int radius){
     vector<Point> curve;

    for (int i = 0; i <= 360; i += 1) {
        double angle = i * CV_PI / 180;
        int x = center.x + radius * cos(angle);
        int y = center.y + radius * sin(angle);
        curve.push_back(Point(x, y));
    }

    return curve;
}

double points_distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}


double contour_area(vector<Point> points, int n_points)
{
    double area = 0.0;
    // shoelace formula calculation:
    // 1/2 ((x1y2 + x2y3 + … + xn-1yn + xny1) – (x2y1 + x3y2 + … + xnyn-1 + x1yn))
    int j = n_points - 1;
    for (int i = 0; i < n_points; i++)
    {
        area += (points[j].x + points[i].x) * (points[j].y - points[i].y);
        j = i;
    }
    return abs(area / 2.0);
}


double contour_perimeter(vector<Point> points, int n_points)
{
    double distance_sum = 0;
    for (int i = 0; i < n_points; i++)
    {
        int next_point = i + 1;
        if (i == n_points - 1)
        {
            next_point = 0;
        }

        double distance = points_distance(points[i].x, points[i].y, points[next_point].x, points[next_point].y);
        distance_sum += distance;
    }
    return distance_sum;
}

void display_area_perimeter(vector<Point> curve, Mat &outputImg){

    double area = contour_area(curve, curve.size());
    double perimeter = contour_perimeter(curve, curve.size());

    string string_area, string_perimeter;
    string_area = to_string(area);
    string_perimeter = to_string(perimeter);
    putText(outputImg, "area = "+ string_area, Point(5,15), FONT_HERSHEY_PLAIN, 1, Scalar(0,255,0), 1, false);
    putText(outputImg, "perimeter = " + string_perimeter, Point(5,35), FONT_HERSHEY_PLAIN, 1, Scalar(0,255,0), 1, false);

}


vector<Point> active_Contour_Model(Mat img, Mat &outputImg, Point center, int radius, int numIterations, double alpha, double beta, double gamma){

    vector<Point> curve = initial_contour(center, radius);
    snake(img, curve, numIterations, alpha, beta, gamma);

    outputImg = img.clone();
    int i;
    for (i = 0; i < curve.size(); i++) {
        circle(outputImg, curve[i], 4, Scalar(0, 0, 255), -1);
        if (i > 0) {
            line(outputImg, curve[i-1], curve[i], Scalar(255, 0, 0), 2);
        }
    }
    line(outputImg, curve[0], curve[i-1], Scalar(255, 0, 0), 2);

    display_area_perimeter(curve,outputImg);

    return curve;
}






/*
  1- This is a helper function that takes a point p and a direction direction
  2- Returns the neighbor of point p in the given direction (E , NE, N , NW , W , SW, S ).
*/

Point getNeighbor(const Point &p, ChainCode direction)
{
    switch (direction)
    {
    case East:
        return {p.x + 1, p.y};
    case NorthEast:
        return {p.x + 1, p.y - 1};
    case North:
        return {p.x, p.y - 1};
    case NorthWest:
        return {p.x - 1, p.y - 1};
    case West:
        return {p.x - 1, p.y};
    case SouthWest:
        return {p.x - 1, p.y + 1};
    case South:
        return {p.x, p.y + 1};
    case SouthEast:
        return {p.x + 1, p.y + 1};
    }
}

/*
1- This is function takes an image and a point.
2-  Returns the value of the pixel at that point in the image.
*/
int pixelValue(const std::vector<std::vector<int>> &image, const Point &p)
{
    return image[p.y][p.x];
}

/*
 1- This  function  takes a current direction.
 2- Returns the next direction in the clockwise order.
*/
ChainCode findNextDirection(ChainCode currentDirection)
{
    return static_cast<ChainCode>((currentDirection + 1) % 8);
}

/*
 1- This function takes an image and a starting point.
 2- Returns the chain code of the contour starting from that point.
 3- It uses the getNeighbor, pixelValue, and findNextDirection helper functions to traverse the contour in a clockwise direction.
 4- Adds each direction to a vector of chain codes.
*/
std::vector<ChainCode> chainCode(const std::vector<std::vector<int>> &image, const Point &startPoint)
{
    std::vector<ChainCode> contour;
    Point currentPoint = startPoint;
    ChainCode currentDirection = East;
    Point nextPoint;
    int maxIterations = image.size() * image[0].size(); // maximum number of iterations to avoid infinite loop

    do
    {
        // Find the next boundary point by checking the neighboring points in the clockwise order
        ChainCode direction = currentDirection;
        do
        {
            nextPoint = getNeighbor(currentPoint, direction);
            direction = findNextDirection(direction);
        } while (pixelValue(image, nextPoint) == 0 && direction != currentDirection);

        // Add the direction to the contour
        contour.push_back(direction);

        // Update the current point and direction
        currentPoint = nextPoint;
       currentDirection = findNextDirection(static_cast<ChainCode>(direction + 4)); // opposite direction of the current direction
    } while (currentPoint != startPoint && contour.size() < maxIterations);

    return contour;
}

/*
1- This function  takes an image.
2- Returns a suitable starting point for the chain code algorithm.
3- It simply returns the first non-zero pixel found.
*/
Point findStartingPoint(const std::vector<std::vector<int>> &image)
{
    // Simply return the first non-zero pixel found
    for (int y = 0; y < image.size(); ++y)
    {
        for (int x = 0; x < image[y].size(); ++x)
        {
            if (image[y][x] != 0)
            {
                return {x, y};
            }
        }
    }
    // Return the origin if no non-zero pixel is found
    return {0, 0};
}

/*
1- This function  takes a chain code.
2- Rotates it to a canonical representation.
3- It finds the minimum chain code value in the contour.
4- Rotates the contour so that the minimum value is at the beginning.
*/
std::vector<ChainCode> normalizeContour(const std::vector<ChainCode> &contour)
{
    std::vector<ChainCode> normalized = contour;
    int minIndex = 0;
    for (int i = 1; i < contour.size(); ++i)
    {
        if (contour[i] < normalized[minIndex])
        {
            minIndex = i;
        }
    }
    std::rotate(normalized.begin(), normalized.begin() + minIndex, normalized.end());
    return normalized;
}

/*

1- This function takes a list of boundary points.
2- creates a binary image from them.
3- It creates a 2D vector of zeros with the specified width and height.
4- sets the pixel values at the boundary points to 1.

*/
std::vector<std::vector<int>> createImageFromBoundary(const std::vector<Point> &boundary, int size)
{
    std::vector<std::vector<int>> image(size, std::vector<int>(size, 0));
    for (const Point &p : boundary)
    {
        if (p.x >= 0 && p.x < size && p.y >= 0 && p.y < size)
        {
            image[p.y][p.x] = 1;
        }
    }
    return image;
}

// ------------------------------------- Main function ------------------------------
//int main()
//{
//    // Input boundary points
//    std::vector<Point> boundary = active_Contour_Model();

//    // Create a binary image from the boundary points
//    int width = 360;
//    int height = 360;
//    std::vector<std::vector<int>> image = createImageFromBoundary(boundary, width, height);

//    // Find the starting point
//    Point startPoint = findStartingPoint(image);

//    // Calculate the chain code
//    std::vector<ChainCode> contour = chainCode(image, startPoint);

//    // Normalize and print the chain code
//    std::vector<ChainCode> normalizedContour = normalizeContour(contour);
//    for (ChainCode cc : normalizedContour)
//    {
//        std::cout << cc << " ";
//    }
//    std::cout << std::endl;

//    return 0;
//}



