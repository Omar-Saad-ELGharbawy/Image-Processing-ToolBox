#include <iostream>
#include <opencv2/opencv.hpp>
#include "Harris.hpp"

using namespace std;
using namespace cv;


Mat Harris_operator(Mat img){

    // Convert image to grayscale
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // Compute image derivatives
    Mat dx, dy;
    Sobel(gray, dx, CV_32FC1, 1, 0);
    Sobel(gray, dy, CV_32FC1, 0, 1);

    // Compute structure tensor elements
    Mat Ixx = dx.mul(dx);
    Mat Ixy = dx.mul(dy);
    Mat Iyy = dy.mul(dy);

    // Compute sum of structure tensor elements in local neighborhood
    int blockSize = 3;
    Mat Sxx, Sxy, Syy;
    boxFilter(Ixx, Sxx, CV_32FC1, Size(blockSize,blockSize));
    boxFilter(Ixy, Sxy, CV_32FC1, Size(blockSize,blockSize));
    boxFilter(Iyy, Syy, CV_32FC1, Size(blockSize,blockSize));

    // Compute Harris response
    Mat det, trace, response;
    det = Sxx.mul(Syy) - Sxy.mul(Sxy);
    trace = Sxx + Syy;
    response = det - 0.04 * trace.mul(trace);

    // Normalize response to 0-255 range
    normalize(response, response, 0, 255, NORM_MINMAX, CV_32FC1, Mat());

    // Threshold response to find corners
    // Mat dst;
    // dst = Mat::zeros(response.size(), CV_8UC1);
    Mat dst = img.clone();

    for(int i = 0; i < response.rows; i++)
    {
        for(int j = 0; j < response.cols; j++)
        {
            if((int)response.at<float>(i,j) > 100)
            {
                circle(dst, Point(j,i), 5, Scalar(0, 0, 255), 2, 8, 0);
            }
        }
    }

    return dst;

}
