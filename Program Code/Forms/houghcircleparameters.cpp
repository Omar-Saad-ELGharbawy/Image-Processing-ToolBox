#include "houghcircleparameters.h"
#include "ui_houghcircleparameters.h"
#include <QMessageBox>

HoughCircleParameters::HoughCircleParameters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HoughCircleParameters)
{
    ui->setupUi(this);
}

HoughCircleParameters::~HoughCircleParameters()
{
    if(ui->ThresholdValue->text().isEmpty() || ui->MinRadiusValue->text().isEmpty() || ui->MaxRadiusValue->text().isEmpty() || ui->CannyMaxValue->text().isEmpty() || ui->CannyMinValue->text().isEmpty()) {
        flag = true;
    }
    delete ui;

}


void HoughCircleParameters::on_Apply_clicked()
{
    if(ui->ThresholdValue->text().isEmpty() || ui->MinRadiusValue->text().isEmpty() || ui->MaxRadiusValue->text().isEmpty() || ui->CannyMaxValue->text().isEmpty() || ui->CannyMinValue->text().isEmpty())
    {
        QMessageBox::information(this, "Warning", "Please enter all the fields before applying!");
    }
    else{
        thresholdValue = ui->ThresholdValue->text().toInt();
        minimumRadius = ui->MinRadiusValue->text().toInt();
        maximumRadius = ui->MaxRadiusValue->text().toInt();
        cannyMaximumThreshold = ui->CannyMaxValue->text().toInt();
        cannyMinimumThreshold = ui->CannyMinValue->text().toInt();
        flag = false;
        this->close();
    }
}

