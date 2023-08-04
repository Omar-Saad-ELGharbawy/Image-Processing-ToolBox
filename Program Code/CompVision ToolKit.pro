QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += C:\openCV\opencv\release\install\include

LIBS += C:\openCV\opencv\release\bin\libopencv_core470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_highgui470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_imgcodecs470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_imgproc470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_calib3d470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_calib3d470.dll
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ActiveContour/activecontour.cpp \
    Agglomerative/agglomerative.cpp \
    Features/Harris.cpp \
    Features/image.cpp \
    Features/matching.cpp \
    Features/sift.cpp \
    Filters/add_noise.cpp \
    Filters/convolution.cpp \
    Filters/edge_detection.cpp \
    Filters/helper_functions.cpp \
    Filters/remove_noise.cpp \
    Forms/houghcircleparameters.cpp \
    Forms/meanshiftparameters.cpp \
    Frequency/frequency.cpp \
    Histogram/Histogram.cpp \
    HoughTransfrom/hough_transform.cpp \
    K-means-Segmentation/k-means.cpp \
    Mean-Shift-Segmentation/meanShift.cpp \
    Region-Growing-Segmentation/regiongrowing.cpp \
    Threshold/Thresholding.cpp \
    UI/cannyparameters.cpp \
    UI/clickablelabel.cpp \
    UI/gaussiannoiseparameters.cpp \
    UI/saltpepperparameters.cpp \
    UI/thresholdwindow.cpp \
    UI/uniformnoiseparameters.cpp \
    imageClass.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ActiveContour/activecontour.h \
    Agglomerative/agglomerative.h \
    Common.hpp \
    Features/Harris.hpp \
    Features/image.hpp \
    Features/matching.hpp \
    Features/sift.hpp \
    Features/stb_image.h \
    Features/stb_image_write.h \
    Filters/add_noise.hpp \
    Filters/convolution.hpp \
    Filters/edge_detection.hpp \
    Filters/helper_functions.hpp \
    Filters/remove_noise.hpp \
    Forms/houghcircleparameters.h \
    Forms/meanshiftparameters.h \
    Frequency/frequency.hpp \
    Histogram/Histogram.hpp \
    HoughTransfrom/hough_transform.hpp \
    K-means-Segmentation/k-means.h \
    Mean-Shift-Segmentation/meanShift.hpp \
    Region-Growing-Segmentation/regiongrowing.hpp \
    Threshold/Thresholding.hpp \
    UI/cannyparameters.h \
    UI/clickablelabel.h \
    UI/gaussiannoiseparameters.h \
    UI/saltpepperparameters.h \
    UI/thresholdwindow.h \
    UI/uniformnoiseparameters.h \
    imageClass.hpp \
    mainwindow.h

FORMS += \
    Forms/cannyparameters.ui \
    Forms/gaussiannoiseparameters.ui \
    Forms/houghcircleparameters.ui \
    Forms/meanshiftparameters.ui \
    Forms/saltpepperparameters.ui \
    Forms/thresholdwindow.ui \
    Forms/uniformnoiseparameters.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    Icons/DropDownIcon.png \
    Icons/FilterIcon.png \
    Icons/Histogram.png \
    Icons/NoiseIcon.png \
    Icons/SIFT.png \
    Icons/Sidebar.png \
    Icons/Snake.png \
    Icons/Threshold.jpg \
    Icons/UploadIcon.png \
    Icons/alpha.png \
    Icons/beta.png \
    Icons/close.png \
    Icons/contrast.png \
    Icons/convert_color_to_gray.png \
    Icons/edge_detection.png \
    Icons/gamma.png \
    Icons/hybrid.png \
    Icons/sidebar-2-512.png \
    Images/Cat.jpeg \
    Images/Dog.jpeg \
    Images/Haley-Powers-photo-1546921919-d445a1a0c10c.jpg \
    Images/Kalen-Emsley-photo-1464822759023-fed622ff2c3b.jpg \
    Images/Kara-Eads-photo-1554995207-c18c203602cb.jpg \
    Images/Matteo-Catanese-photo-1518391846015-55a9cc003b25.jpg \
    Images/building.png \
    Images/fingerprint.jpeg \
    Images/girl.jpg \
    Images/hough_images/circles.jpg \
    Images/hough_images/circles_ex1.png \
    Images/hough_images/circles_ex2.png \
    Images/hough_images/circles_ex3.png \
    Images/hough_images/coins.png \
    Images/hough_images/hough_img.png \
    Images/hough_images/lines_ex1.png \
    Images/hough_images/lines_ex2.png \
    Images/hough_images/lines_ex3.png \
    Images/lenna.png \
    Images/sudoku.png \
    Images/tiger.jpg


RESOURCES += \
    Fonts.qrc \
    Fonts.qrc \
    Fonts.qrc \
    Fonts.qrc \
    Fonts.qrc \
    Fonts.qrc \
    Fonts.qrc \
    Fonts.qrc \
    Icons.qrc \
    Icons.qrc \
    Icons.qrc \
    Icons.qrc \
    Icons.qrc \
    Icons.qrc \
    Icons.qrc \
    Icons.qrc


