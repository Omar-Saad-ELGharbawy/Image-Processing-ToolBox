#include "clickablelabel.h"
#include <algorithm>
#include "qevent.h"
#include "ui_mainwindow.h"
#include <mainwindow.h>
#include <ActiveContour/activecontour.h>
#include <Region-Growing-Segmentation/regiongrowing.hpp>

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {

    MainWindow *Obj;
    xFactor = ClickableLabel::width() / 512.0;
    yFactor = ClickableLabel::height() / 512.0;

    QPoint point = event->pos();   

    xCoordinate = qCeil(point.x() / xFactor);
    yCoordinate = qCeil(point.y() / yFactor);

    // Get minimum distance between center and image boundaries
    minX = min(512 - xCoordinate, xCoordinate);
    minY = min(512 - yCoordinate, yCoordinate);

    Obj->ui->contourRadiusSlider->setMaximum( min(minX,minY) );
    active_Contour_Model(Obj->inputMat, Obj->activeContourOutputMat, Point(xCoordinate, yCoordinate), Obj->radius, Obj->numIterations, Obj->alpha, Obj->beta, Obj->gamma);
    Obj->updateImage(Obj->activeContourOutputMat, Obj->ui->activeContourOutputImage, 1);


    isMouseClicked = true;
    emit clicked();
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent* event){
    isMouseClicked = false;
}
