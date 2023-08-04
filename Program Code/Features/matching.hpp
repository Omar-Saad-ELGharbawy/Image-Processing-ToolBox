#ifndef MATCHING
#define MATCHING

#include "common.hpp"
#include <opencv2/imgcodecs.hpp>
#include <vector>

// #include <iostream>
// #include <opencv2/opencv.hpp>
// using namespace std;
// using namespace cv;




double Sum_of_Squared_Differences(const  vector<double> &a, const  vector<double> &b);

double Normalized_Cross_Correlation(const  vector<double> &a, const  vector<double> &b);

void matchFeatures(const  vector< vector<double>> &features1, const  vector< vector<double>> &features2, vector< pair<int, int>> &matches, bool use_ncc , double threshold);

vector< vector<double>> matrix_to_vectors(const  vector< vector<double>> &matrix);

Mat Matching(Mat &img1, Mat &img2, bool matching_type, double threshold );

#endif
