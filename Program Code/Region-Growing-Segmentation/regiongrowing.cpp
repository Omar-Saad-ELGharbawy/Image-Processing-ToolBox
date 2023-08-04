
/* required libraries */
#include "regiongrowing.hpp"



/*  ------------------------------------------------------------------------------------------------------
this 2 functions :
  * Get the number of rows, columns , and the required region matrix for segmentation.
  * colorize this region according to the label given to it.
  * support grayscale and colored images.
*/


Mat colorize_Region_Gray_Scale_Img(int rows, int cols, Mat &labelMatrix, Mat &outputImage)
{

    // Colorize the regions in the output image
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Get the label of the current pixel in the label matrix
            int label = labelMatrix.at<uchar>(i, j);
            if (label != 0)
            {
                // Colorize the pixel with a unique color based on its label
                outputImage.at<Vec3b>(i, j) = Vec3b((label * 50) % 255, (label * 100) % 255, (label * 150) % 255);
            }
        }
    }

    return outputImage;
}




Mat colorize_Region_Color_Scale_Img(int rows, int cols, Mat &inputImage, Mat &labelMatrix, Mat &outputImage)
{

    // Assign the colors to the output image based on the label matrix
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (labelMatrix.at<uchar>(i, j) == 1)
            {
                outputImage.at<Vec3b>(i, j) = inputImage.at<Vec3b>(i, j);
            }
        }
    }
    return outputImage;
}






/*  ------------------------------------------------------------------------------------------------------
this 2functions :
  * Get the image, the vector of seed points, the intial label matrix, and the threshold from the user as input.
  * start from a given seed point and iterate over 8-directions pixel according to a given thresold.
  * if the difference is less than thresold , add this point to this region with its label.
  * add this point also to the queue of seed points to iterate around it also.
*/

Mat region_gray_img_det(std::queue<Point> q, int rows, int cols, Mat &inputImage, Mat &labelMatrix, int threshold)
{

    int newLabel = 0;

    // check if the queue has at least on seed point
    while (!q.empty())
    {
        // Get the pixel at the front of the queue, which is the next pixel to be checked.
        Point currentPoint = q.front();
        // Removes the pixel , so that the next iteration of the loop will process the next pixel in the queue.
        q.pop();

        // Check the 8-connected neighbors of the current point
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {

                // Skip the current point and out-of-bounds pixels
                if (i == 0 && j == 0)
                    continue;

                int x = currentPoint.x + i;
                int y = currentPoint.y + j;

                if (x < 0 || x >= cols || y < 0 || y >= rows)
                    continue;

                // Check if the neighbor is within the threshold
                int difference = abs(inputImage.at<uchar>(currentPoint) - inputImage.at<uchar>(y, x));
                if (difference <= threshold && labelMatrix.at<uchar>(y, x) == 0)
                {
                    // Assign a new label to the neighbor and add it to the queue
                    newLabel = labelMatrix.at<uchar>(currentPoint);
                    labelMatrix.at<uchar>(y, x) = newLabel;
                    q.push(Point(x, y));
                }
            }
        }
    }

    return labelMatrix;
}





Mat region_color_img_det(std::queue<Point> q, int rows, int cols, Mat &inputImage, Mat &labelMatrix, int threshold)
{

    int newLabel = 0;

    while (!q.empty())
    {
        // Get the next point from the queue
        Point currentPoint = q.front();
        q.pop();

        // Get the pixel value at the current point
        Vec3b currentValue = inputImage.at<Vec3b>(currentPoint);

        // Check the 8-connected neighborhood of the current point.
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                // Skip the current point
                if (i == 0 && j == 0)
                    continue;

                // Get the coordinates of the neighboring point
                int x = currentPoint.x + j;
                int y = currentPoint.y + i;

                // Check if the neighboring point is within the image boundaries
                if (x >= 0 && y >= 0 && x < cols && y < rows)
                {
                    // Get the pixel value at the neighboring point
                    Vec3b neighborValue = inputImage.at<Vec3b>(y, x);


                    // Calculate the absolute difference between the current pixel value and the neighboring pixel value
                    int diff = abs(currentValue[0] - neighborValue[0]) + abs(currentValue[1] - neighborValue[1]) + abs(currentValue[2] - neighborValue[2]);

                    // Check if the difference is within the threshold
                    if (diff <= threshold)
                    {
                        // Check if the neighboring point has already been labeled
                        if (labelMatrix.at<uchar>(y, x) == 0)
                        {
                            // Assign the current label to the neighboring point
                            labelMatrix.at<uchar>(y, x) = 1;

                            // Add the neighboring point to the queue
                            q.push(Point(x, y));
                        }
                    }
                }
            }
        }
    }
    return labelMatrix;
}


/*  ------------------------------------------------------------------------------------------------------
this function :
  * Get the image, the vector of seed points , and the threshold from the user as input.
  * define the output image matrix with intial condition zeros .
  * define the matrix that hold the required region labels with intial condition zeros.
  * define the queue that will hold the seed points over every iteration.
  * add the first seed point to the queue.
  * call the functions for iterations and colorization.
*/

Mat regionGrowingHelper(Mat &inputImage, Point &seedPoint, int threshold, int labelNumber, int colorflag)
{

    Mat regionlabeledMatrix;

    // no. of image rows
    int rows = inputImage.rows;

    // no. of image columns
    int cols = inputImage.cols;

    // Create an empty label matrix
    Mat labelMatrix = Mat::zeros(rows, cols, CV_8UC1);

    // Create a queue for storing the neighboring points to be checked
    std::queue<Point> q;

    // Initialize the queue with the seed point and set its label
    q.push(seedPoint);

    // Assigns a new label to the seed point in the labelMatrix matrix.
    labelMatrix.at<uchar>(seedPoint) = labelNumber;

    

    if (colorflag == 0)
    {
        // Convert the image to grayscale
        Mat grayImage;
        cvtColor(inputImage, grayImage, COLOR_BGR2GRAY);

        regionlabeledMatrix = region_gray_img_det(q, rows, cols, grayImage, labelMatrix, threshold);

        // Create an output image to show the segmented regions
        Mat outputImage = Mat::zeros(grayImage.size(), CV_8UC3);

        Mat Imgcolorized = colorize_Region_Gray_Scale_Img(rows, cols, regionlabeledMatrix, outputImage);

        return Imgcolorized;
    }
    else{

        regionlabeledMatrix = region_color_img_det(q, rows, cols, inputImage, labelMatrix, threshold);

        // Create an output image to show the segmented regions
        Mat outputImage = Mat::zeros(inputImage.size(), inputImage.type());

        Mat Imgcolorized = colorize_Region_Color_Scale_Img(rows, cols ,inputImage, regionlabeledMatrix, outputImage);

        return Imgcolorized;

    }

}

/*  ------------------------------------------------------------------------------------------------------
this function :
  * Get the image, the vector of seed points , and the threshold from the user as input
  * Iterate over each seed point and send it to the main function for the process
  * the interation use the number of seed points have been sent from the user
  * for each new region it increment the region label number
*/
Mat regionGrowingMultiSeed(Mat &inputImage, vector<Point> &seedPoints, int threshold, int colorflag)
{
    Mat outputImage;

    // Initialize the label number to 1
    int labelNumber = 1;

    // Process on each seed point
    for (Point seedPoint : seedPoints)
    {
        // Call the regionGrowingHelper function to grow the region around the seed point
        outputImage = regionGrowingHelper(inputImage, seedPoint, threshold, labelNumber, colorflag);
        // Increment the label number for the next seed point / region
        labelNumber++;
    }

    return outputImage;
}
