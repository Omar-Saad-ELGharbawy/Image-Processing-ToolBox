#ifndef MEANSHIFTPARAMETERS_H
#define MEANSHIFTPARAMETERS_H

#include <QDialog>

namespace Ui {
class MeanShiftParameters;
}

class MeanShiftParameters : public QDialog
{
    Q_OBJECT

public:
    explicit MeanShiftParameters(QWidget *parent = nullptr);
    ~MeanShiftParameters();

    int ColorBandwidthValue;
    int SpatialBandwidthValue;
    bool flag = true;

private slots:
    void on_pushButton_clicked();

private:
    Ui::MeanShiftParameters *ui;
};

#endif // MEANSHIFTPARAMETERS_H
