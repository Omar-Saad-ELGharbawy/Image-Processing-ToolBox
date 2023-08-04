#ifndef REGIONGROWING
#define REGIONGROWING

#include "Common.hpp"

Mat regionGrowingHelper(Mat &inputImage, Point &seedPoint , int threshold, int labelNumber, int colorflag);

Mat regionGrowingMultiSeed(Mat &inputImage, vector<Point> &seedPoints, int threshold, int colorflag);

Mat colorize_Region_Gray_Scale_Img(int rows, int cols, Mat &labelMatrix , Mat &outputImage);

Mat region_gray_img_det( std::queue<Point> q , int rows, int cols , Mat &inputImage , Mat &labelMatrix ,int threshold);

Mat region_color_img_det( std::queue<Point> q , int rows, int cols , Mat &inputImage , Mat &labelMatrix ,int threshold);

Mat colorize_Region_Color_Scale_Img(int rows, int cols, Mat &inputImage, Mat &labelMatrix, Mat &outputImage);


#endif
