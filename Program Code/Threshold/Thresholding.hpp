#ifndef THRESHOLDING
#define THRESHOLDING

#include "Common.hpp"


Mat global_threshold(Mat src, Mat dst, unsigned char threshold_value, unsigned char maximum_value = 255, unsigned char minimum_value = 0);

Mat local_adaptive_threshold(Mat src, Mat dst, unsigned char kernal_size = 3, unsigned char C = 2, unsigned char maximum_value = 255, unsigned char minimum_value = 0);

Mat adaptive_padding_function(Mat src);

Mat local_threshold_mean_calculation(const Mat &src, int target_app = 1, int C = 2);

Mat otsu_thresholding (Mat image);

map<int, int> histo (Mat image);

Mat thresholding(Mat gray_image, int threshold);

Mat optimal_thresholding (Mat image);

Mat Apply_Fourier_Transform( Mat &src);

Mat Inverse_Fourier_Transform( Mat &src);

Mat spectral_thresholding (Mat image);

Mat double_thresholding(Mat gray_image, int min_threshold, int max_threshold);

Mat spectral_localThresholding (Mat image);
#endif
