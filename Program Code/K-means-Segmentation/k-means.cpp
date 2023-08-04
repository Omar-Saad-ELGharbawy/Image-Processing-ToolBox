#include <iostream>
#include <opencv2/opencv.hpp>
#include "k-means.h"

using namespace std;
using namespace cv;

// ---------------------------------------------------------------------------------- //
// --------------------------- Gray K-means Function -------------------------------- //
// ---------------------------------------------------------------------------------- //
Mat Gray_kmeans_segmentation(Mat image, int k) {
    // Convert the image to a 2D array of floats
    Mat data;
    image.convertTo(data, CV_32FC3, 1.0 / 255);
    int rows = data.rows;
    int cols = data.cols;

    // Initialize the cluster centers randomly
    RNG rng(0);
    vector<Point3f> centers(k);
    for (int i = 0; i < k; i++) {
        int x = rng.uniform(0, cols);
        int y = rng.uniform(0, rows);
        centers[i] = data.at<Point3f>(y, x);
    }

    // Assign each pixel to the nearest cluster center
    Mat labels(rows, cols, CV_32S);
    for (int iter = 0; iter < 10; iter++) {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                Point3f pixel = data.at<Point3f>(r, c);
                int best_label = 0;
                float best_dist = FLT_MAX;
                for (int i = 0; i < k; i++) {
                    float dist = norm(pixel - centers[i]);
                    if (dist < best_dist) {
                        best_dist = dist;
                        best_label = i;
                    }
                }
                labels.at<int>(r, c) = best_label;
            }
        }

        // Update the cluster centers as the mean of the assigned pixels
        vector<Point3f> new_centers(k, Point3f(0, 0, 0));
        vector<int> counts(k, 0);
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                Point3f pixel = data.at<Point3f>(r, c);
                int label = labels.at<int>(r, c);
                new_centers[label] += pixel;
                counts[label] += 1;
            }
        }
        for (int i = 0; i < k; i++) {
            if (counts[i] > 0) {
                new_centers[i] /= counts[i];
            }
        }
        centers = new_centers;
    }

    // Create the segmented image
    Mat segmented_image(rows, cols, CV_8UC1);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int label = labels.at<int>(r, c);
            segmented_image.at<uchar>(r, c) = static_cast<uchar>(label * 255 / (k - 1));
        }
    }

    return segmented_image;
}

// ---------------------------------------------------------------------------------- //
// ---------------------------- RGB K-means Function -------------------------------- //
// ---------------------------------------------------------------------------------- //
Mat RGB_kmeans_segmentation(Mat image, int k) {
    int rows = image.rows;
    int cols = image.cols;

    // Convert the image to a 2D array of floats
    Mat data(rows * cols, 3, CV_32F);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Vec3b pixel = image.at<Vec3b>(r, c);
            data.at<float>(r * cols + c, 0) = pixel[0];
            data.at<float>(r * cols + c, 1) = pixel[1];
            data.at<float>(r * cols + c, 2) = pixel[2];
        }
    }

    // Initialize the cluster centers randomly
    RNG rng(0);
    vector<Vec3f> centers(k);
    for (int i = 0; i < k; i++) {
        int index = rng.uniform(0, rows * cols);
        Vec3f pixel = data.at<Vec3f>(index, 0);
        centers[i] = pixel;
    }

    // Assign each pixel to the nearest cluster center
    Mat labels(rows * cols, 1, CV_32S);
    for (int iter = 0; iter < 10; iter++) {
        for (int i = 0; i < rows * cols; i++) {
            Vec3f pixel = data.at<Vec3f>(i, 0);
            int best_label = 0;
            float best_dist = FLT_MAX;
            for (int j = 0; j < k; j++) {
                float dist = norm(pixel - centers[j]);
                if (dist < best_dist) {
                    best_dist = dist;
                    best_label = j;
                }
            }
            labels.at<int>(i, 0) = best_label;
        }

        // Update the cluster centers as the mean of the assigned pixels
        vector<Vec3f> new_centers(k, Vec3f(0, 0, 0));
        vector<int> counts(k, 0);
        for (int i = 0; i < rows * cols; i++) {
            Vec3f pixel = data.at<Vec3f>(i, 0);
            int label = labels.at<int>(i, 0);
            new_centers[label] += pixel;
            counts[label] += 1;
        }
        for (int i = 0; i < k; i++) {
            if (counts[i] > 0) {
                new_centers[i] /= counts[i];
            }
        }
        centers = new_centers;
    }

    // Create the segmented image by mapping each cluster label to a color
    Mat segmented_image(rows, cols, CV_8UC3);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int index = r * cols + c;
            int label = labels.at<int>(index, 0);
            Vec3f color = centers[label];
            segmented_image.at<Vec3b>(r, c)[0] = static_cast<uchar>(color[0]);
            segmented_image.at<Vec3b>(r, c)[1] = static_cast<uchar>(color[1]);
            segmented_image.at<Vec3b>(r, c)[2] = static_cast<uchar>(color[2]);
        }
    }

    return segmented_image;
}