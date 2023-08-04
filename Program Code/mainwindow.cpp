#include "mainwindow.h"
#include "ui_mainwindow.h"



#include "Filters/add_noise.hpp"
#include "Filters/remove_noise.hpp"
#include "Filters/edge_detection.hpp"

#include "Frequency/frequency.hpp"
#include "Threshold/Thresholding.hpp"
#include "Histogram/Histogram.hpp"
#include "ActiveContour/activecontour.h"
#include "HoughTransfrom/hough_transform.hpp"
#include "Mean-Shift-Segmentation/meanShift.hpp"
#include "K-means-Segmentation/k-means.h"
#include "Agglomerative/agglomerative.h"

#include "Features/Harris.hpp"
#include "Features/sift.hpp"

#include <iostream>
#include <QFileDialog>
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    ui->pushButton->setDisabled(true);

    ui->GlobalThresholdSlider->setHidden(true);
    ui->HoughTransfromSlider->setHidden(true);
    ui->ThresholdSlider->setDisabled(true);

    ui->stackedWidget->setCurrentIndex(0);
    ui->filtering_tab_btn->setChecked(true);

}


MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_BrowseButton_clicked()
{
    UploadImage(inputImage, inputMat, true, TRASH);

    if(inputImage.isNull()) return;

    Convert_To_Gray(inputMat, filterOutputMat);
    Convert_To_Gray(inputMat, edgeDetectionOutputMat);


    updateImage(inputMat, ui->filter_inputImage, 1);
    updateImage(inputMat, ui->EdgeDetection_inputImage, 1);
    updateImage(inputMat, ui->Threshold_InputImage, 1);
    updateImage(inputMat, ui->activeContourInputImage, 1);
    updateImage(inputMat, ui->HoughTransfromInputImage, 1);
    updateImage(inputMat, ui->Contrast_InputImage, 1);
    updateImage(inputMat, ui->Segmentation_InputImage, 1);


    updateImage(inputMat, ui->activeContourOutputImage, 1);
    updateImage(filterOutputMat, ui->filter_outputImage, 0);

    updateImage(inputMat, ui->features_inputImage, 1);


}



// ------------------------------------------------ FILTERING & NOISE TAB --------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------------


/*******************************************************************************
 *                                Filtering                                    *
 *******************************************************************************/


void MainWindow::on_GrayScale_clicked()
{
    if(checkImage(inputImage)) return;
    Convert_To_Gray(inputMat, filterOutputMat);
    updateImage(filterOutputMat, ui->filter_outputImage, 0);

}


void MainWindow::on_MedianFilterButton_clicked()
{
    if(checkImage(inputImage)) return;

    Add_Median_Filter(filterOutputMat, filterOutputMat, kernalSize);
    updateImage(filterOutputMat, ui->filter_outputImage, 0);

}


void MainWindow::on_AverageFilterButton_clicked()
{
    if(checkImage(inputImage)) return;

    Add_Average_Filter(filterOutputMat, filterOutputMat, kernalSize);
    updateImage(filterOutputMat, ui->filter_outputImage, 0);
}

void MainWindow::on_GaussianFilterButton_clicked()
{
    if(checkImage(inputImage)) return;

    filterOutputMat = Gaussian_Filter(filterOutputMat, kernalSize);
    updateImage(filterOutputMat, ui->filter_outputImage, 0);
}


/*******************************************************************************
 *                                Noise                                        *
 *******************************************************************************/

void MainWindow::on_SaltPepperNoiseButton_clicked()
{
    if(checkImage(inputImage)) return;

    SaltPepperParameters SaltPepperParameter;
    SaltPepperParameter.setModal(true);
    SaltPepperParameter.exec();

    if(SaltPepperParameter.flag) return;

    Add_Salt_And_Pepper_Noise(filterOutputMat, filterOutputMat, SaltPepperParameter.saltAndPepperAmount);
    updateImage(filterOutputMat, ui->filter_outputImage, 0);
}


void MainWindow::on_GaussianNoiseButton_clicked()
{
    if(checkImage(inputImage)) return;

    GaussianNoiseParameters gaussianParameters;
    gaussianParameters.setModal(true);
    gaussianParameters.exec();

    if(gaussianParameters.flag) return;

    cout << gaussianParameters.noiseIntenisty << endl;
    Add_Gaussian_Noise(filterOutputMat, filterOutputMat, gaussianParameters.gaussianMeanValue, gaussianParameters.gaussianSTDValue, gaussianParameters.noiseIntenisty);
    updateImage(filterOutputMat, ui->filter_outputImage, 0);
}

void MainWindow::on_UniformNoiseButton_clicked()
{
    if(checkImage(inputImage)) return;

    UniformNoiseParameters uniformParameters;
    uniformParameters.setModal(true);
    uniformParameters.exec();

    if(uniformParameters.flag) return;

    Add_Uniform_Noise(filterOutputMat, filterOutputMat, uniformParameters.noiseIntenisty);
    updateImage(filterOutputMat, ui->filter_outputImage, 0);
}

void MainWindow::on_Radio3x3Kernal_clicked()
{
    kernalSize = 3;
}


void MainWindow::on_Radio5x5Kernal_clicked()
{
    kernalSize = 5;
}


// ----------------------------------------------------------- EDGE DETECTION TAB ------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------------


void MainWindow::on_prewittButton_clicked()
{
    if(checkImage(inputImage)) return;
    Convert_To_Gray(inputMat, edgeDetectionOutputMat);
    edgeDetectionOutputMat = Detect_Edges_Prewitt(edgeDetectionOutputMat);
    updateImage(edgeDetectionOutputMat, ui->EdgeDetection_outputImage, 0);
}


void MainWindow::on_robertButton_clicked()
{
    if(checkImage(inputImage)) return;
    Convert_To_Gray(inputMat, edgeDetectionOutputMat);
    edgeDetectionOutputMat = Detect_Edges_Robert(edgeDetectionOutputMat);
    updateImage(edgeDetectionOutputMat, ui->EdgeDetection_outputImage, 0);
}


void MainWindow::on_sobelButton_clicked()
{
    if(checkImage(inputImage)) return;
    Convert_To_Gray(inputMat, edgeDetectionOutputMat);



    edgeDetectionOutputMat = Detect_Edges_Sobel(edgeDetectionOutputMat);
    updateImage(edgeDetectionOutputMat, ui->EdgeDetection_outputImage, 0);
}


void MainWindow::on_cannyButton_clicked()
{
    if(checkImage(inputImage)) return;

    CannyParameters cannyParameters;
    cannyParameters.setModal(true);
    cannyParameters.exec();

    if(cannyParameters.flag) return;

    Convert_To_Gray(inputMat, edgeDetectionOutputMat);
    edgeDetectionOutputMat = Detect_Edges_Canny(edgeDetectionOutputMat, cannyParameters.LowThresholdValue, cannyParameters.HighThresholdValue);
    updateImage(edgeDetectionOutputMat, ui->EdgeDetection_outputImage, 0);
}


// ----------------------------------------------------------- CONTRAST TAB ------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------

/*******************************************************************************
 *                               Global Threshold                              *
 *******************************************************************************/
void MainWindow::on_GlobalThresholdButton_clicked()
{
    if(checkImage(inputImage)) return;

    ui->GlobalThresholdSlider->setHidden(false);

    Convert_To_Gray(inputMat, globalThresholdOutputMat);


    globalThresholdOutputMat = global_threshold(globalThresholdOutputMat, globalThresholdOutputMat, globalThresholdSliderValue);
    updateImage(globalThresholdOutputMat, ui->Contrast_OutputImage, 0);

}

void MainWindow::on_GlobalThresholdSlider_valueChanged(int value)
{
    Convert_To_Gray(inputMat, globalThresholdOutputMat);

    globalThresholdSliderValue = value;
    globalThresholdOutputMat = global_threshold(globalThresholdOutputMat, globalThresholdOutputMat, globalThresholdSliderValue);

    updateImage(globalThresholdOutputMat, ui->Contrast_OutputImage, 0);
}

/*******************************************************************************
 *                               Normalization                                 *
 *******************************************************************************/

void MainWindow::on_NormalizeButton_clicked()
{
    if(checkImage(inputImage)) return;

    ui->GlobalThresholdSlider->setHidden(true);

    Convert_To_Gray(inputMat, normalizedOutputMat);
    normalizedOutputMat = normalize_image(normalizedOutputMat);
    updateImage(normalizedOutputMat, ui->Contrast_OutputImage, 0);
}


/*******************************************************************************
 *                               Equalization                                  *
 *******************************************************************************/
void MainWindow::on_EqualizeButton_clicked()
{
    if(checkImage(inputImage)) return;

    ui->EqualizeLabel->setHidden(false);
    ui->GlobalThresholdSlider->setHidden(true);

    Convert_To_Gray(inputMat, equalizedOutputMat);
    equalizedOutputMat = Equalize_img(equalizedOutputMat);
    updateImage(equalizedOutputMat, ui->Contrast_OutputImage, 0);
}


// ----------------------------------------------------------- HYBRID FILTER TAB ------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_UploadeImage1_clicked()
{
    UploadImage(hybridImage1, hybridImage1Mat, false, TRASH);
    if(hybridImage1.isNull()) return;

    updateFrequencyResponse(hybridImage1Mat, freqImage1Mat, ui->freqOutputImage1, freqImage1Slider, image1_H_L);
    updateImage(hybridImage1Mat, ui->hybridInputImage1, 1);
}


void MainWindow::on_UploadeImage2_clicked()
{
    UploadImage(hybridImage2, hybridImage2Mat, false, TRASH);
    if(hybridImage2.isNull()) return;

    updateFrequencyResponse(hybridImage2Mat, freqImage2Mat, ui->freqOutputImage2, freqImage2Slider, image2_H_L);
    updateImage(hybridImage2Mat,  ui->hybridInputImage2, 1);
}


/*******************************************************************************
 *                               UI_Slider Output                              *
 *******************************************************************************/
void MainWindow::on_Image1FSlider_valueChanged(int value)
{
    image2_H_L = value ^ 1;
    ui->Image2FSlider->setSliderPosition(image2_H_L);

    if(hybridImage1.isNull()) return;
    updateFrequencyResponse(hybridImage1Mat, freqImage1Mat, ui->freqOutputImage1, freqImage1Slider, value);
}


void MainWindow::on_Image2FSlider_valueChanged(int value)
{
    image1_H_L = value ^ 1;
    ui->Image1FSlider->setSliderPosition(image1_H_L);

    if(hybridImage2.isNull()) return;
    updateFrequencyResponse(hybridImage2Mat, freqImage2Mat, ui->freqOutputImage2, freqImage2Slider, value);
}

void MainWindow::on_freqOutputImage1Slider_valueChanged(int value)
{
    freqImage1Slider = value;

    if(hybridImage1.isNull()) return;
    updateFrequencyResponse(hybridImage1Mat, freqImage1Mat, ui->freqOutputImage1, freqImage1Slider, image1_H_L);

}

void MainWindow::on_freqOutputImage2Slider_valueChanged(int value)
{
    freqImage2Slider = value;

    if(hybridImage2.isNull()) return;
    updateFrequencyResponse(hybridImage2Mat, freqImage2Mat, ui->freqOutputImage2, freqImage2Slider, image2_H_L);
}


void MainWindow::on_HybridButton_clicked()
{
    if(hybridImage2.isNull() && hybridImage1.isNull()) return;
    finalHybridImageMat =  Apply_Hybrid_Images(freqImage1Mat, freqImage2Mat, image1_H_L);
    updateImage(finalHybridImageMat,  ui->finalHybridImage, 0);
}

// ----------------------------------------------------------- Active Contour Tab ------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------



void MainWindow::on_alphaSlider_valueChanged(int value)
{
    alpha = value/10.0;

    if(inputImage.isNull()) return;
    updateActiveContour(inputMat, activeContourOutputMat);


}


void MainWindow::on_betaSlider_valueChanged(int value)
{
    beta = value/10.0;

    if(inputImage.isNull()) return;
    updateActiveContour(inputMat, activeContourOutputMat);
}


void MainWindow::on_gammaSlider_valueChanged(int value)
{
    gamma = value/10.0;

    if(inputImage.isNull()) return;
    updateActiveContour(inputMat, activeContourOutputMat);
}

void MainWindow::on_contourRadiusSlider_valueChanged(int value)
{
    radius = value;

    if(inputImage.isNull()) return;
    updateActiveContour(inputMat, activeContourOutputMat);
}



// ----------------------------------------------------------- HOUGH TRANSFROM TAB ---------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_HoughLineButton_clicked()
{
    if(checkImage(inputImage)) return;

    ui->HoughTransfromSlider->setHidden(false);

    circleFlag = false;
    cv::resize(inputMat, houghTransfromOutputMat, cv::Size(512,512), 0, 0);

    houghTransfromOutputMat = Hough_Line_Transform(inputMat, houghThreshold);
    updateImage(houghTransfromOutputMat, ui->HoughTransfromOutputImage, 1);
}


void MainWindow::on_HoughCircleButton_clicked()
{
    if(checkImage(inputImage)) return;

    ui->HoughTransfromSlider->setHidden(true);

    HoughCircleParameters houghCircleParameters;
    houghCircleParameters.setModal(true);
    houghCircleParameters.exec();

    if(houghCircleParameters.flag) return;


    cv::resize(inputMat, houghTransfromOutputMat, cv::Size(300,300), 0, 0);

    houghTransfromOutputMat = Hough_Circle_Transform(houghTransfromOutputMat, houghCircleParameters.thresholdValue, houghCircleParameters.minimumRadius, houghCircleParameters.maximumRadius, houghCircleParameters.cannyMinimumThreshold, houghCircleParameters.cannyMaximumThreshold);
    updateImage(houghTransfromOutputMat, ui->HoughTransfromOutputImage, 1);
}


void MainWindow::on_HoughTransfromSlider_valueChanged(int value)
{
    if(inputImage.isNull()) return;

    houghThreshold = value;

    cv::resize(inputMat, houghTransfromOutputMat, cv::Size(512,512), 0, 0);
    houghTransfromOutputMat = Hough_Line_Transform(inputMat, houghThreshold);

    updateImage(houghTransfromOutputMat, ui->HoughTransfromOutputImage, 1);
}


// ----------------------------------------------------------- FEATURES TAB ------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------


/*******************************************************************************
 *                               Harris                                        *
 *******************************************************************************/
void MainWindow::on_harrisButton_clicked()
{
    if(checkImage(inputImage)) return;

    harrisOutputMat = Harris_operator(inputMat);
    updateImage(harrisOutputMat, ui->features_outputImage, 1);

}

/*******************************************************************************
 *                               SIFT                                          *
 *******************************************************************************/

void MainWindow::on_UploadeSiftImage1_clicked()
{
    UploadImage(siftinputImage1, siftinputImageMat1, false, imgPathSift1);
    if(siftinputImage1.isNull()) return;

    updateImage(siftinputImageMat1,  ui->sift_inputImage1, 1);

}

void MainWindow::on_UploadeSiftImage2_clicked()
{
    UploadImage(siftinputImage2, siftinputImageMat2, false, imgPathSift2);
    if(siftinputImage2.isNull()) return;

    updateImage(siftinputImageMat2,  ui->sift_InputImage2, 1);

}

void MainWindow::on_siftButton_clicked()
{
    if(checkImage(siftinputImage1) && checkImage(siftinputImage2)) return;

    Image image1(imgPathSift1.toStdString());
    Image image2(imgPathSift2.toStdString());

    image1 =  image1.channels == 1 ? image1 : rgb_to_grayscale(image1);
    image2 =  image2.channels == 1 ? image2 : rgb_to_grayscale(image2);

    auto start = std::chrono::high_resolution_clock::now();
    // call your function
    std::vector<sift::Keypoint> kps1 = sift::find_keypoints_and_descriptors(image1);
    std::vector<sift::Keypoint> kps2 = sift::find_keypoints_and_descriptors(image2);
    // end time measurement
    auto end = std::chrono::high_resolution_clock::now();



    std::vector<std::pair<int, int>> matches = sift::find_keypoint_matches(kps1, kps2, 0.7,  350, 1);

    Image result = sift::draw_matches_rect(image1, image2, kps1, kps2, matches);
    Mat featuresMatching = result.save("result.jpg");
    cv::resize(featuresMatching,featuresMatching,cv::Size(500,300));

    // compute and print the elapsed time
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Time taken by function: " << duration.count()*0.000001 << " seconds" << std::endl;

    updateImage(featuresMatching, ui->sift_outputImage_2, 1);
}


void MainWindow::on_ThresholdSlider_valueChanged(int value)
{
    thresholdSliderValue = value/10.0;
}


// ------------------------------------------------ THRESHOLD TAB --------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------

/*******************************************************************************
 *                                Local Threshold                              *
 *******************************************************************************/
void MainWindow::on_LocalThresholdButton_clicked()
{
    if(checkImage(inputImage)) return;

    thresholdOutputMat = spectral_thresholding(inputMat);
    updateImage(thresholdOutputMat, ui->Threshold_OutputImage, 0);
}

/*******************************************************************************
 *                                Optimal Threshold                              *
 *******************************************************************************/
void MainWindow::on_OptimalThresholdButton_clicked()
{
    if(checkImage(inputImage)) return;

    thresholdOutputMat = optimal_thresholding(inputMat);
    updateImage(thresholdOutputMat, ui->Threshold_OutputImage, 0);
}

/*******************************************************************************
 *                                OTSU Threshold                              *
 *******************************************************************************/
void MainWindow::on_OTSUThresholdButton_clicked()
{
    if(checkImage(inputImage)) return;

    thresholdOutputMat = otsu_thresholding(inputMat);
    updateImage(thresholdOutputMat, ui->Threshold_OutputImage, 0);
}

/*******************************************************************************
 *                                Spectral Threshold                              *
 *******************************************************************************/
void MainWindow::on_SpectralThresholdButton_clicked()
{
    if(checkImage(inputImage)) return;

    thresholdOutputMat = spectral_thresholding(inputMat);
    updateImage(thresholdOutputMat, ui->Threshold_OutputImage, 0);

}

// ----------------------------------------------------------- SEGMENTATION TAB ------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_RegionGrowingButton_clicked()
{
    ui->agglomerativeSliderValue->setHidden(true);

    updateImage(inputMat, ui->Segmentation_OutputImage, 1);
    MODE = 2;

}

void MainWindow::on_MeanShiftButton_clicked()
{
    MODE = 0;
    ui->agglomerativeSliderValue->setHidden(true);
    if(checkImage(inputImage)) return;

    MeanShiftParameters meanShiftParameters;
    meanShiftParameters.setModal(true);
    meanShiftParameters.exec();

    if(meanShiftParameters.flag) return;

    segmentationOutputMat = meanShift_Segmentation(inputMat, meanShiftParameters.SpatialBandwidthValue, meanShiftParameters.ColorBandwidthValue);
    updateImage(segmentationOutputMat, ui->Segmentation_OutputImage, 1);

}


void MainWindow::on_KMeanButton_clicked()
{
    MODE = 0;
    ui->agglomerativeSliderValue->setHidden(true);
    if(checkImage(inputImage)) return;

    segmentationOutputMat = RGB_kmeans_segmentation(inputMat, 5);
    updateImage(segmentationOutputMat, ui->Segmentation_OutputImage, 1);
}

void MainWindow::on_AgglomerativeButton_clicked()
{
    MODE = 0;

    ui->agglomerativeSliderValue->setHidden(false);
    if(checkImage(inputImage)) return;

    tie(agglomerativePixels, agglomerativeResizedImage) = image_preperation(inputMat);
    finalAgglomerativeImage = image_color_segmentation(numberOfClusters ,agglomerativePixels, agglomerativeResizedImage);
    updateImage(finalAgglomerativeImage, ui->Segmentation_OutputImage, 1);
}

void MainWindow::on_agglomerativeSliderValue_valueChanged(int value)
{
    numberOfClusters = value;

    cout << numberOfClusters << endl;

    tie(agglomerativePixels, agglomerativeResizedImage) = image_preperation(inputMat);
    finalAgglomerativeImage = image_color_segmentation(numberOfClusters ,agglomerativePixels, agglomerativeResizedImage);
    updateImage(finalAgglomerativeImage, ui->Segmentation_OutputImage, 1);
}


// ----------------------------------------------------------- HELPER FUNCTIONS ------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

bool MainWindow::checkImage(QImage &image)
{
    if(image.isNull()){
        QMessageBox::information(this, "Image not uploaded", "Please upload image first!");
        return true;
    }
    else return false;
}


void MainWindow::UploadImage(QImage &image, Mat &imageMat, bool flag, QString &imgPath)
{
    imgPath = QFileDialog::getOpenFileName(this,tr("Open image"));

    reader.setFileName(imgPath);
    image = reader.read();

    if(!image.isNull() && flag){
    ui-> BrowseButton -> setText("Uploaded");
    ui-> BrowseButton -> setStyleSheet("QPushButton{border-radius: 10px; text-align: left; font: 900 12pt 'Segoe UI Black'; color: green} QPushButton:hover:!pressed{background-color: qlineargradient(x1: 0.5, y1: 1, x2: 0.5, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa)}");
    ui-> BrowseButton -> setDisabled(true);
    ui-> pushButton-> setDisabled(false);
    }

    image = image.convertToFormat(QImage::Format_BGR888);
    imageMat = Mat(image.height(), image.width(), CV_8UC3, image.bits(), image.bytesPerLine());

    if(image.isNull()) return;
    cv::resize(imageMat, imageMat, cv::Size(512,512), 0, 0);

}

void MainWindow::on_pushButton_clicked()
{
    inputImage =  QImage();
    ui-> BrowseButton -> setText("Upload Image");
    ui-> BrowseButton -> setStyleSheet("QPushButton{border-radius: 10px; text-align: left; font: 900 12pt 'Segoe UI Black';} QPushButton:hover:!pressed{background-color: qlineargradient(x1: 0.5, y1: 1, x2: 0.5, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa)}");
    ui->BrowseButton->setEnabled(true);
    ui->pushButton->setDisabled(true);

    ui->filter_inputImage->clear();
    ui->filter_outputImage->clear();

    ui->EdgeDetection_inputImage->clear();
    ui->EdgeDetection_outputImage->clear();

    ui->Threshold_InputImage->clear();
    ui->Threshold_OutputImage->clear();

    ui->Contrast_OutputImage->clear();
    ui->Contrast_InputImage->clear();

    ui->GlobalThresholdSlider->setHidden(true);


    ui->activeContourInputImage->clear();
    ui->activeContourOutputImage->clear();

    ui->HoughTransfromSlider->setHidden(true);
    ui->HoughTransfromInputImage->clear();
    ui->HoughTransfromOutputImage->clear();

    ui->features_outputImage->clear();
    ui->features_inputImage->clear();

    ui->Segmentation_InputImage->clear();
    ui->Segmentation_OutputImage->clear();

    ui->ThresholdSlider->setDisabled(true);
}


void MainWindow::updateImage(Mat &inputMat,  QLabel* image, bool rgb_flag){

    if(rgb_flag){
        image->setPixmap(QPixmap::fromImage(QImage(inputMat.data, inputMat.cols, inputMat.rows, inputMat.step, QImage::Format_BGR888)));
    }
    else{
        image->setPixmap(QPixmap::fromImage(QImage(inputMat.data, inputMat.cols, inputMat.rows, inputMat.step, QImage::Format_Grayscale8)));
    }
}


void MainWindow::updateFrequencyResponse(Mat &inputMat, Mat &freqMat, QLabel* image, int sliderValue, int high_low_flag){
    Convert_To_Gray(inputMat, freqMat);
    freqMat = Add_Low_High_Frequency_Filter(freqMat, sliderValue, high_low_flag);
    image->setPixmap(QPixmap::fromImage(QImage(freqMat.data, freqMat.cols, freqMat.rows, freqMat.step, QImage::Format_Grayscale8)));
}

void MainWindow::updateActiveContour(Mat &inputMat, Mat &outputMat){

    vector<Point> boundary = active_Contour_Model(inputMat, outputMat, Point(xCoordinate, yCoordinate), radius, numIterations, alpha, beta, gamma);
    vector<std::vector<int>> image = createImageFromBoundary(boundary, boundary.size());

    // Find the starting point
    Point startPoint = findStartingPoint(image);

    // Calculate the chain code
    vector<ChainCode> contour = chainCode(image, startPoint);

    // Normalize and print the chain code
    std::vector<ChainCode> normalizedContour = normalizeContour(contour);
    cout << "ChainCode = ";
    for (ChainCode cc : normalizedContour)
    {
        cout << cc << " ";
    }
    cout << endl;

    updateImage(outputMat, ui->activeContourOutputImage, 1);
}




void MainWindow::on_filtering_tab_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_edge_detection_tab_btrn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_constrast_tab_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_hybrid_tab_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_active_contour_tab_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    MODE = 1;
}

void MainWindow::on_hough_transfrom_tab_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_sift_tab_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_harris_tab_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_threshold_tab_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_segmentation_tab_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}








