#include <opencv2/opencv.hpp>
#include <cmath>
#include <stdio.h>

using namespace std;
using namespace cv;


Mat RGB_kmeans_segmentation(Mat image, int k);
Mat Gray_kmeans_segmentation(Mat image, int k);