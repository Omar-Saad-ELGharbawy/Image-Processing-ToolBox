# Image Processing Application

## Table of contents:
- [Introduction](#introduction)
- [Project Features](#project-features)
- [Project Structure](#project-structure)
- [Quick Preview](#quick-preview)
- [Requirements to run](#Requirements-to-run)
- [Try a demo](#Try-a-demo)
- [Reports](#Reports)
- [Team]()


### Introduction
It’s an image processing implementation functions project implemented in C++ with a desktop application (Qt) that consists of five tabs that let the user add noise to an image, filter the added noise, view different types of histograms, apply thresholds to an image, and create hybrid images. 

### Project Features
In this project you can:
- [x] Add different types of noise to the image:
  - Salt and Paper noise.
  - Uniform noise.
  - Gaussian noise.
- [x] Using different types of filtering:
  - Average filter .
  - Gaussian filter.
  - Median filter.
- [x] Detect edges in the image using:
  - Sobel edge detector.
  - Roberts edge detector.
  - Prewitt edge detector.
  - Canny edge detector.
- [x] Draw Histograms and Distribution curves for the uploaded image.
- [x] Equalize and Normalize the image.
- [x] Transform the image from color to gray scale image and plot Red, Green, and Blue histograms with their cumulative curves.
- [x] Implement Filtering in the Frequency Domain.
  - Ideal Low Pass filter (smoothing).
  - Ideal High Pass filter (sharpening).
- [x] Implement Corner Detection Technique (Harris Operator)
- [x] Implement SIFT Algorithm
- [x] Implement Thresholding Techniques such as:
  - Optimal
  - Otsu
  - Global Spectral
  - Local Spectral
- [x] Implement various types of segmenations such as:
  - K-Means Segmentation
  - Region Growing Segmentation
  - Agglomerative Segmentation
  - Mean Shift Segmentation


### Project Structure
The ToolKit is built using:
- C++/OpenCV:
  - OpenCV 14/15/16 versions

- QT framework:
  - QT 6.4 version or above
 
 - Python
   -  [ Python Notebook ](https://github.com/Dinahussam/Impro-App/blob/main/Python%20Notebook/Task%20in%20python.ipynb)
    - For visualization and comparing implmented algorithms results by built in functions. 


### Quick Preview

#### Add different types of noises to the image and filtering them.
![Filter Tab](https://github.com/MohamedAIsmail/CompVision-ToolKit/blob/main/Gifs/FiltersTab.gif)
#### Detect edges in the image using edge detectors
![Edge Detection Tab](https://github.com/MohamedAIsmail/CompVision-ToolKit/blob/main/Gifs/EdgeDetection.gif)
#### Contrast Enhancing Tab
![Contrast Enhancing Tab](https://github.com/MohamedAIsmail/CompVision-ToolKit/blob/main/Gifs/ContrastEnhancing.gif)
#### Low and High pass filtering and create Hybrid images (Old UI).
![Hybrid Tab](https://user-images.githubusercontent.com/94166833/225074411-a0d8a5cd-3f5b-4b2c-b8b3-4e44015ed536.gif)
#### Active Contour on Images
![Active Contour Tab](https://github.com/MohamedAIsmail/CompVision-ToolKit/blob/main/Gifs/ActiveContour.gif)
#### Hough Transfrom 
![Hough Transfrom Tab](https://github.com/MohamedAIsmail/CompVision-ToolKit/blob/main/Gifs/Hough%20Transfrom.gif)
#### Applying Thresholding on Images
![Threshold Tab](https://github.com/MohamedAIsmail/CompVision-ToolKit/blob/main/Gifs/Thresholding.gif)
#### Applying Segmentation Techniques on Images
![Segmentation Tab](https://github.com/MohamedAIsmail/CompVision-ToolKit/blob/main/Gifs/Segmentation.gif)


### Requirements to run 

[ Qt Setup and openCV ](https://github.com/Dinahussam/Impro-App/files/10972282/Qt.Setup.and.openCV.pdf)


### Try a demo

[ Download Here !](https://www.mediafire.com/file/xefpiecn6spkh7p/ToolKit-Setup.exe/file)

### Reports
You can find detailed reports about each algorithm implemented in
this project [here](https://github.com/Omar-Saad-ELGharbawy/Image_Processing/tree/main/Reports)


### Team

Second Semester - Biomedical Computer Vision (SBE3230) class project created by:

| Team Members' Names                                  | Section | B.N. |
|------------------------------------------------------|:-------:|:----:|
| [Dina Hussam](https://github.com/Dinahussam)         |    1    |  28  |
| [Omar Ahmed ](https://github.com/omaranwar21)        |    2    |  2   |
| [Omar saad ](https://github.com/Omar-Saad-ELGharbawy)|    2    |  3   |
| [Mohamed Ahmed](https://github.com/MohamedAIsmail)   |    2    |  16  |
| [Neveen Mohamed](https://github.com/NeveenMohamed)   |    2    |  49  |
