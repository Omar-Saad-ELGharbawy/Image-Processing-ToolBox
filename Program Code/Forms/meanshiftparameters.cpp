#include "meanshiftparameters.h"
#include "ui_meanshiftparameters.h"
#include <QMessageBox>

MeanShiftParameters::MeanShiftParameters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeanShiftParameters)
{
    ui->setupUi(this);
}

MeanShiftParameters::~MeanShiftParameters()
{
    if(ui->ColorBandwidthValue->text().isEmpty() || ui->SpatialBandwidthValue->text().isEmpty()) {
        flag = true;
    }
    delete ui;
}

void MeanShiftParameters::on_pushButton_clicked()
{
    if(ui->ColorBandwidthValue->text().isEmpty() || ui->SpatialBandwidthValue->text().isEmpty())
    {
        QMessageBox::information(this, "Warning", "Please enter all the fields before applying!");
    }
    else{
        ColorBandwidthValue = ui->ColorBandwidthValue->text().toInt();
        SpatialBandwidthValue = ui->SpatialBandwidthValue->text().toInt();
        flag = false;
        this->close();
    }
}

