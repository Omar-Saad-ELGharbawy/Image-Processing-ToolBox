#include "matching.hpp"


#include <cmath>
#include <chrono>



//----------------------------------- Sum of Squared Differences Matching -----------------------------------
/*
  Function to compute SSD between two feature descriptors :
  1- loop on both vectors to get the values
  2- get the difference between each two vector points
  3- add the result to a sum variable to get the score output
  4- return the sum

*/

double Sum_of_Squared_Differences(const std::vector<double> &a, const std::vector<double> &b)
{

    double sum = 0.0;

    for (int i = 0; i < a.size(); i++)
    {
        double diff = a[i] - b[i];
        sum += (diff * diff);
    }

    return sum;
}

//----------------------------------  Normalized Cross Correlation --------------------------------------

double Normalized_Cross_Correlation(const std::vector<double> &a, const std::vector<double> &b)
{
    // auto start_time = std::chrono::high_resolution_clock::now(); // start measuring time

    double mean1 = 0.0;
    double mean2 = 0.0;
    double sum1 = 0.0;
    double sum2 = 0.0;
    double sum3 = 0.0;

    /*
      1- loop on each point in the two vectors
      2- get each point and store it in the value variable
      3- add the value into the mean variable
     */

    for (int i = 0; i < a.size(); i++)
    {
        double value1 = a[i];
        double value2 = b[i];
        mean1 += value1;
        mean2 += value2;
    }

    mean1 /= (a.size());
    mean2 /= (b.size());

    /*
      1- loop on each point in the two vectors
      2- get the value of each point and store it in the value variable
      3- calculate sum1, sum2 and sum3
     */

    for (int i = 0; i < a.size(); i++)
    {
        double value1 = a[i];
        double value2 = b[i];
        sum1 += (value1 - mean1) * (value2 - mean2);
        sum2 += (value1 - mean1) * (value1 - mean1);
        sum3 += (value2 - mean2) * (value2 - mean2);
    }

    // auto end_time = std::chrono::high_resolution_clock::now();                                    // end measuring time
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time); // compute duration
    // std::cout << "NCC computation time: " << duration.count() << " microseconds" << std::endl;    // print duration

    if (sum2 == 0.0 || sum3 == 0.0)
    {
        return 0.0; // or any other default value
    }

    return (sum1) / (sqrt(sum2 * sum3));
}

// Function to match features using SSD or NCC ---------------------------------------------------------------
void matchFeatures(const std::vector<std::vector<double>> &features1, const std::vector<std::vector<double>> &features2,std::vector<std::pair<int, int>> &matches, bool use_ncc , double threshold)
{
    
    auto start_time = std::chrono::high_resolution_clock::now(); // start measuring time

    for (int i = 0; i < features1.size(); i++)
    {
        int best_match_idx = -1;

        double best_match_score = use_ncc ? -1.0 : std::numeric_limits<double>::max(); // initialize score
        for (int j = 0; j < features2.size(); j++)
        {
            double score = use_ncc ? Normalized_Cross_Correlation(features1[i], features2[j])
                                   : Sum_of_Squared_Differences(features1[i], features2[j]);
            if (score < threshold)
                continue; // skip if below threshold
            if (use_ncc && score > best_match_score || !use_ncc && score < best_match_score)
            {
                best_match_idx = j;
                best_match_score = score;
            }
        }
        if (best_match_idx != -1)
        {
            matches.push_back(std::make_pair(i, best_match_idx));
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();                                    // end measuring time
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time); // compute duration
    std::cout << "Computation time: " << duration.count() << " microseconds" << std::endl;    // print duration
}

std::vector<std::vector<double>> matrix_to_vectors(const std::vector<std::vector<double>> &matrix)
{
    std::vector<std::vector<double>> result(matrix[0].size(), std::vector<double>(matrix.size()));

    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            result[j][i] = matrix[i][j];
        }
    }

    return result;
}

//Mat Matching(Mat &img1, Mat &img2, bool matching_type, double threshold )
//{

//    Mat gray1, gray2;
//    resize(img1, img1, Size(512, 512), 0, 0, INTER_LINEAR);
//    resize(img2, img2, Size(512, 512), 0, 0, INTER_LINEAR);

//    cvtColor(img1, gray1, COLOR_BGR2GRAY);
//    cvtColor(img2, gray2, COLOR_BGR2GRAY);

//    // keypoints
//    Ptr<SIFT> sift = SIFT::create();

//    vector<KeyPoint> keypoints_1;
//    Mat descriptors_1;
//    sift->detectAndCompute(img1, noArray(), keypoints_1, descriptors_1);


//    vector<KeyPoint> keypoints_2;
//    Mat descriptors_2;
//    sift->detectAndCompute(img2, noArray(), keypoints_2, descriptors_2);

//    // Convert Mat to vector<vector<float>>
//    vector<vector<double>> desc_vector1(descriptors_1.rows, vector<double>(descriptors_1.cols));
//    for (int i = 0; i < descriptors_1.rows; i++)
//    {
//        for (int j = 0; j < descriptors_1.cols; j++)
//        {
//            desc_vector1[i][j] = descriptors_1.at<float>(i, j);
//        }
//    }

//    vector<vector<double>> desc_vector2(descriptors_2.rows, vector<double>(descriptors_2.cols));
//    for (int i = 0; i < descriptors_2.rows; i++)
//    {
//        for (int j = 0; j < descriptors_2.cols; j++)
//        {
//            desc_vector2[i][j] = descriptors_2.at<float>(i, j);
//        }
//    }

//    Mat img_1;
//    Mat img_2;
//    drawKeypoints(gray1, keypoints_1, img_1);
//    drawKeypoints(gray2, keypoints_2, img_2);

//    imshow("Keypoints 1", img_1);
//    imshow("Keypoints 2", img_2);


//    std::vector<std::pair<int, int>> matches_ssd;
//    matchFeatures(desc_vector1, desc_vector2, matches_ssd, matching_type ,threshold);
//    std::cout << "Number of matches : " << matches_ssd.size() << std::endl;

   
//    std::vector<DMatch> matches;

//    for (const auto &match : matches_ssd)
//    {
//        DMatch dmatch;
//        dmatch.queryIdx = match.first;
//        dmatch.trainIdx = match.second;
//        matches.push_back(dmatch);
//    }
//        //  Sort matches based on distance
//    std::sort(matches.begin(), matches.end(), [](const cv::DMatch &a, const cv::DMatch &b)
//              { return a.distance < b.distance; });
//    // Visualize the matches using OpenCV
//    cv::Mat matches_image;
//    cv::drawMatches(img1, keypoints_1, img2, keypoints_2, matches, matches_image);

//    return matches_image;
//}
