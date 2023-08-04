#ifndef HOUGHCIRCLEPARAMETERS_H
#define HOUGHCIRCLEPARAMETERS_H

#include <QDialog>

namespace Ui {
class HoughCircleParameters;
}

class HoughCircleParameters : public QDialog
{
    Q_OBJECT

public:
    explicit HoughCircleParameters(QWidget *parent = nullptr);
    ~HoughCircleParameters();

    int thresholdValue;
    int minimumRadius;
    int maximumRadius;
    int cannyMinimumThreshold;
    int cannyMaximumThreshold;
    bool flag = true;


private slots:
    void on_Apply_clicked();

private:
    Ui::HoughCircleParameters *ui;
};

#endif // HOUGHCIRCLEPARAMETERS_H
