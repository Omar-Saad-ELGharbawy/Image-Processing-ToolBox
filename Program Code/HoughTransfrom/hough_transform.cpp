#include "hough_transform.hpp"

/************************************************************************************************************
 *                      Functions Definitions                                                               *
 ************************************************************************************************************/

/*************************************************************************************************
 ******************                        Hough Line                                 ************
 *************************************************************************************************/


Mat Hough_Line_Transform ( const Mat &image , int threshold )
{

     // Convert the image to grayscale
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);
    // Perform edge detection
    Mat edges;
    Canny(grayImage, edges, 150, 200);

    int rows = grayImage.rows;
    int cols = grayImage.cols;

    // Define Hough parameters
    int max_rho = sqrt(rows*rows + cols*cols);
    int rho_step = 1;
    double theta_step = CV_PI / 180;
    int theta_bins = 180;
    int rho_bins = 2 * max_rho / rho_step + 1;
    // int threshold = 120;

    // Initialize accumulator
    vector<vector<int>> accumulator(rho_bins, vector<int>(theta_bins));

    // Loop over edge points
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            if (edges.at<uchar>(y, x) > 0)
            {
                // Loop over theta values
                for (int t = 0; t < theta_bins; t++)
                {
                    double theta = t * theta_step;
                    double rho = x * cos(theta) + y * sin(theta);
                    int rho_bin = round((rho + max_rho) / rho_step);
                    accumulator[rho_bin][t]++;
                }
            }
        }
    }

    // Find lines in accumulator
    vector<Vec2f> lines;
    for (int r = 1; r < rho_bins -1; r++)
    {
        for (int t = 1; t < theta_bins -1; t++)
        {
            // Check For accumulator threshold
            if (accumulator[r][t] > threshold)
            {
                // Check if local maximum
                if (accumulator[r][t] > accumulator[r-1][t-1] &&
                    accumulator[r][t] > accumulator[r-1][t] &&
                    accumulator[r][t] > accumulator[r-1][t+1] &&
                    accumulator[r][t] > accumulator[r][t-1] &&
                    accumulator[r][t] > accumulator[r][t+1] &&
                    accumulator[r][t] > accumulator[r+1][t-1] &&
                    accumulator[r][t] > accumulator[r+1][t] &&
                    accumulator[r][t] > accumulator[r+1][t+1])
                {
                    double rho = (r * rho_step) - max_rho;
                    double theta = t * theta_step;
                    lines.push_back(Vec2f(rho, theta));
                }
            }
        }
    }

    // Draw lines on original image
    Mat result = image.clone();
    for (size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + max_rho  * (-b));
        pt1.y = cvRound(y0 + max_rho * (a));
        pt2.x = cvRound(x0 - max_rho * (-b));
        pt2.y = cvRound(y0 - max_rho * (a));
        line(result, pt1, pt2, Scalar(0, 0, 255), 1, LINE_AA);
    }

//    namedWindow("Edges", WINDOW_AUTOSIZE );
//    imshow("Edges", edges);
    return result;

}

/*************************************************************************************************
 ******************                        Hough Circle                               ************
 *************************************************************************************************/


Mat Hough_Circle_Transform ( const Mat &image , int threshold , int min_radius /* =10 */ , int max_radius /* =200 */ , int canny_min_thresold /* = 100 */ , int canny_max_thresold /* = 200 */ )
{
    // Convert the image to grayscale
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);
    // Perform edge detection
    Mat edges;
    Canny(grayImage, edges, canny_min_thresold, canny_max_thresold);

    // Perform Hough Circles Transform

    // rows is image height , cols is image width
    int rows = grayImage.rows;
    int cols = grayImage.cols;

    // Define Hough parameters
    int num_thetas = 100;
    double dtheta = (int) (360 / num_thetas);
    int theta_bins = 360;

    // Initialize 3D Accumulator
    vector<vector<vector<int>>> accumulator(cols+1, vector<vector<int>>(rows+1, vector<int>((max_radius-min_radius+1), 0)));

    /* Loop over edge points then raduis and theta values to incrment accumulator */ 
    // Loop over edge points
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            // Check if edge point
            if (edges.at<uchar>(y, x) > 0)
            {
                // Loop over r values
                for (int r = min_radius; r < max_radius; r++)
                {
                    // Loop over theta values
                    for (int theta = 0; theta < num_thetas; theta+= dtheta)
                    {
                        // Calculate x_center and y_center
                        double x_center = x - r * cos(theta * deg2rad);
                        double y_center = y - r * sin(theta * deg2rad);
                        // check that xcenter and ycenter is in valid range
                        if (x_center >= 0 && x_center < cols && y_center >= 0 && y_center < rows)
                        {
                            // Increment accumulator
                            accumulator[(int)x_center][(int)y_center][r-min_radius]++;
                        }
                    }
                }
            }
        }
    }

    Mat circles_img = image.clone();
    // Loop over accumulator to find circles
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            for  (int r = min_radius; r < max_radius; r++)
            {
                // int current_vote = accumulator[x][y][r-min_radius] ;
                // if (current_vote >= threshold) 
                // Check For accumulator threshold
                if (accumulator[x][y][r-min_radius] > threshold)
                {
                // Draw circle
                    circle(circles_img, Point(x , y ), r -min_radius, Scalar(0, 0, 255), 1, LINE_AA);
                    // circles.push_back(Vec3f(x, y , r));
                }
            }
        }
    }
    // namedWindow("Edges", WINDOW_AUTOSIZE );
    // imshow("Edges", edges);
    return circles_img;
}
