#ifndef HOUGH_TRANSFORM
#define HOUGH_TRANSFORM

#include "../Common.hpp"

#define deg2rad (CV_PI/180.0)

/************************************************************************************************************
 *                                    Functions Prototypes                                                  *
 ************************************************************************************************************/


Mat Hough_Line_Transform ( const Mat &image , int threshold );

Mat Hough_Circle_Transform ( const Mat &image , int threshold , int min_radius =10 , int max_radius =200 , int canny_min_thresold = 100 , int canny_max_thresold = 200 );


#endif
