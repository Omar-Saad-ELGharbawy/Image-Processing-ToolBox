                                                                                                                                                                                    #include "Thresholding.hpp"
#include "Filters/convolution.hpp"

Mat global_threshold(Mat src, Mat dst, unsigned char threshold_value, unsigned char maximum_value /* = 255 */, unsigned char minimum_value /* = 0 */)
{
    for (int row = 0; row < src.rows; row++)
    {
        for (int col = 0; col < src.cols; col++)
        {
            if (src.at<uchar>(row, col) >= threshold_value)
            {
                dst.at<uchar>(row, col) = maximum_value;
            }
            else
            {
                dst.at<uchar>(row, col) = minimum_value;
            }
        }
    }
    return dst;
}

Mat local_adaptive_threshold(Mat src, Mat dst, unsigned char kernal_size /* = 3 */, unsigned char C /* = 2 */, unsigned char maximum_value /* = 255 */, unsigned char minimum_value /* = 0 */)
{

    Mat mean_window = local_threshold_mean_calculation(src, 9, C);

    for (int i = 0; i < src.rows; i++)
    {
        for (int  j = 0; j < src.cols; j++)
        {
                if (src.at<uchar>(i, j) >= mean_window.at<uchar>(i, j))
                {
                    dst.at<uchar>(i, j) = maximum_value;
                }
                else
                {
                    dst.at<uchar>(i, j) = minimum_value;
                }
        }
    }

    return dst;
}

Mat adaptive_padding_function(Mat src){

    Mat dst;
    dst = Padd_Mono(src, 1);

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            if( i == 0  && j == 0 ) {
                dst.at<uchar>(i, j) = src.at<uchar>(i, j);
            }
            if(i == src.rows - 1 && j == src.cols - 1){
                dst.at<uchar>(i + 2, j + 2) = src.at<uchar>(i, j);
            }
            if(i == 0 &&  j == src.cols - 1){
                dst.at<uchar>(i , j + 2) = src.at<uchar>(i, j);
            }
            if(i == src.rows - 1 && j == 0){
                dst.at<uchar>(i + 2 , j) = src.at<uchar>(i, j);
            }
            if( i == 0 ){
                dst.at<uchar>(i , j + 1) = src.at<uchar>(i, j);
            }
            if ( j == 0 ){
                dst.at<uchar>(i + 1, j) = src.at<uchar>(i, j);
            }
            if( j == src.cols - 1){
                dst.at<uchar>(i + 1, j + 2) = src.at<uchar>(i, j);
            }
            if( i == src.rows - 1){
                dst.at<uchar>(i + 2 , j + 1) = src.at<uchar>(i, j);
            }
        }
    }

    return dst;
}

Mat local_threshold_mean_calculation(const Mat &src, int target_app, int C)
{
    vector<int> mean_values;
    Mat padded_img = adaptive_padding_function(src);
    Mat convoluted_img = Mat::zeros(Size(src.cols, src.rows), src.type());

    float window_values[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Mat mean_window = Mat(3, 3, CV_32F, window_values);

    int h = mean_window.rows / 2;
    int w = mean_window.cols / 2;

    float window_sum;
    // convolute image
    for (int i = 1; i < (padded_img.rows - 1); i++)
    {
        for (int j = 1; j < (padded_img.cols - 1); j++)
        {
            window_sum = 0;
            for (int m = 0; m < mean_window.rows; m++)
            {
                for (int n = 0; n < mean_window.cols; n++)
                {
                    window_sum += padded_img.at<uchar>(i + m - h, j + n - w) * mean_window.at<float>(m, n);

                }
            }
            convoluted_img.at<uchar>(i - 1, j - 1) = window_sum/target_app - C;

        }
    }
    return convoluted_img;
}

Mat otsu_thresholding (Mat image){
    Mat gray_image;
    cvtColor(image, gray_image, COLOR_BGR2GRAY);
    map<int, int> histogram = histo(gray_image);

    int size = gray_image.rows* gray_image.cols;


    float max_sigma_b = 0;

    int threshold = 0;


//looping on the map
auto it2 = histogram.begin();
    for (auto it = histogram.begin(); it != histogram.end(); it++){
        float w_b = 0;
        float mu_b = 0;
        float w_f = 0;
        float mu_f = 0;
        float sigma_b = 0;
        if (it == histogram.begin()){
            continue;
        }
        for (it2 ; it2 != it; it2++){
            w_b = w_b + it2->second;
            mu_b = mu_b + (it2->first)*(it2->second);
        }
        mu_b = mu_b/w_b;
        w_b = w_b/(size);

        for (auto it3 = it2; it3 != histogram.end(); it3++){
            w_f = w_f + it3->second;
            mu_f = mu_f + (it3->first)*(it3->second);
        }
        it2 = histogram.begin();
        mu_f = mu_f/w_f;
        w_f = w_f/(size);

        sigma_b = w_b*w_f*(mu_b-mu_f)*(mu_b-mu_f);

        if (sigma_b >= max_sigma_b){
            max_sigma_b = sigma_b;
            threshold = it->first;
        }

    }

    Mat threshold_image = thresholding(gray_image, threshold);
    return threshold_image;
}

map<int, int> histo (Mat image){
    map<int, int> histogram;
    for (int i = 0; i < image.rows; i++){
        for (int j = 0; j < image.cols; j++){
            histogram[image.at<uchar>(i, j)]++;
        }
    }
    return histogram;
}

Mat thresholding(Mat gray_image, int threshold){
    Mat threshold_image;
    threshold_image = gray_image.clone();
    for (int row = 0; row < gray_image.rows; row++)
    {
        for (int col = 0; col < gray_image.cols; col++)
        {
            if (gray_image.at<uchar>(row, col) > threshold)
            {
                threshold_image.at<uchar>(row, col) = 255;
            }
            else
            {
                threshold_image.at<uchar>(row, col) = 0;
            }
        }

    }
    return threshold_image;

}

Mat optimal_thresholding (Mat image){
    Mat gray_image;
    cvtColor(image, gray_image, COLOR_BGR2GRAY);
    float threshold_previous;
    float threshold_next;
    vector<int> background;
    vector<int> foreground;
    threshold_next = ((gray_image.at<uchar>(0,0))+ (gray_image.at<uchar>(0,image.rows-1))+(gray_image.at<uchar>(image.rows-1,image.cols-1))+(gray_image.at<uchar>(image.cols-1,0)))/4.0;
    do
    {
        //loop on image
        for (int row = 0; row < gray_image.rows; row++)
        {
            for (int col = 0; col < gray_image.cols; col++)
            {
                if (gray_image.at<uchar>(row, col) > threshold_next)
                {
                    foreground.push_back(gray_image.at<uchar>(row, col));
                }
                else
                {
                    background.push_back(gray_image.at<uchar>(row, col));
                }
            }

        }
        //calculate the mean of background and foreground
        float mean_background = 0;
        float mean_foreground = 0;
        for (int i = 0; i < background.size(); i++){
            mean_background = mean_background + background[i];
        }
        mean_background = mean_background/background.size();
        for (int i = 0; i < foreground.size(); i++){
            mean_foreground = mean_foreground + foreground[i];
        }
        mean_foreground = mean_foreground/foreground.size();
        threshold_previous = threshold_next;
        threshold_next = (mean_background + mean_foreground)/2.0;
        background.clear();
        foreground.clear();
    }while ( abs(threshold_previous - threshold_next) > 0.02 );
    Mat threshold_img = thresholding(gray_image, (int)threshold_next);
    return threshold_img;
}



Mat spectral_thresholding (Mat image){
    // gray imag
    Mat gray_image;
    cvtColor(image, gray_image, COLOR_BGR2GRAY);
    int size = gray_image.rows* gray_image.cols;
    // histogram
    map<int, int> histogram = histo(gray_image);
    // iterate on the map using pointer

    // it_first_threshold++;
    // creat a pointer points to the next record

    // it_second_threshold++;
    // it_second_threshold++;

    auto it_first_threshold = histogram.begin();
    it_first_threshold++;
    // it_first_threshold++;
    auto last_loop_first_threshold = histogram.end();
    last_loop_first_threshold--;
    last_loop_first_threshold--;
    last_loop_first_threshold--;
    last_loop_first_threshold--;

    auto last_loop_second_threshold = histogram.end();
    last_loop_second_threshold--;
    // last_loop_second_threshold--;


    // main variables

    float max_sigma_b = -1;
    int first_threshold;
    int second_threshold;


    do
    {
        auto it_second_threshold = it_first_threshold;
        it_second_threshold++;
        it_second_threshold++;
       do
        {
            float p_1 = 0;
            float m_1 = 0;
            float p_2 = 0;
            float m_2 = 0;
            float p_3 = 0;
            float m_3 = 0;
            float m_G = 0;
            float sigma_b = 0;

            it_first_threshold++;
            for (auto it1 = histogram.begin(); it1 != it_first_threshold; it1++){
                p_1 = p_1 + (it1->second)/(float)size;
                m_1 = m_1 + (it1->first)*((it1->second)/(float)size);
            }
            // it_first_threshold--;
            m_1 = m_1/p_1;
            it_second_threshold++;
            for (auto it2 = it_first_threshold; it2 != it_second_threshold; it2++){
                p_2 = p_2 + (it2->second)/(float)size;
                m_2 = m_2 + (it2->first)*((it2->second)/(float)size);
            }
            // it_second_threshold--;
            m_2 = m_2/p_2;

            for (auto it3 = it_second_threshold; it3 != histogram.end(); it3++){
                p_3 = p_3 + (it3->second)/(float)size;
                m_3 = m_3 + (it3->first)*((it3->second)/(float)size);
            }
            m_3 = m_3/p_3;

            m_G = (p_1*m_1 + p_2*m_2 + p_3*m_3);

            sigma_b = p_1*(m_1-m_G)*(m_1-m_G) + p_2 * (m_2-m_G)*(m_2-m_G) + p_3*(m_3-m_G)*(m_3-m_G);

            it_first_threshold--;
            it_second_threshold--;
            if (sigma_b > max_sigma_b){
                max_sigma_b = sigma_b;
                first_threshold = it_first_threshold->first;
                second_threshold = it_second_threshold->first;
            }
            it_second_threshold++;

        }while (it_second_threshold != last_loop_second_threshold);
        it_first_threshold++;
    }while (it_first_threshold != last_loop_first_threshold);

    cout<< "----------------------" << endl;
    cout<< first_threshold << endl;
    Mat threshold_image = double_thresholding(gray_image, first_threshold, second_threshold);

    return threshold_image;
}


Mat double_thresholding(Mat gray_image, int min_threshold, int max_threshold){
    Mat threshold_image;
    threshold_image = gray_image.clone();
    for (int row = 0; row < gray_image.rows; row++)
    {
        for (int col = 0; col < gray_image.cols; col++)
        {
            if (gray_image.at<uchar>(row, col) > max_threshold)
            {
                threshold_image.at<uchar>(row, col) = 255;
            }
            else if(gray_image.at<uchar>(row, col) < min_threshold)
            {
                threshold_image.at<uchar>(row, col) = 128;
            }else{
                threshold_image.at<uchar>(row, col) = 0;
            }
        }

    }
    return threshold_image;

}

Mat spectral_localThresholding (Mat image){
    int rows = image.rows;
    int cols = image.cols;

    Rect roi1(0, 0, cols/2, rows/2);
    Rect roi2(cols/2, 0, cols/2, rows/2);
    Rect roi3(0, rows/2, cols/2, rows/2);
    Rect roi4(cols/2, rows/2, cols/2, rows/2);

    Mat part1 = image(roi1);
    part1 = spectral_thresholding(part1);
    Mat part2 = image(roi2);
    part2 = spectral_thresholding(part2);
    Mat part3 = image(roi3);
    part3 = spectral_thresholding(part3);
    Mat part4 = image(roi4);
    part4 = spectral_thresholding(part4);

    Mat thresholded_image = image.clone();

    //convert to gray scale
    cvtColor(thresholded_image, thresholded_image, COLOR_BGR2GRAY);

    part1.copyTo(thresholded_image(roi1));
    part2.copyTo(thresholded_image(roi2));
    part3.copyTo(thresholded_image(roi3));
    part4.copyTo(thresholded_image(roi4));

    return thresholded_image;

}
