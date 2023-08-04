#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>
#include <Common.hpp>



class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableLabel();


    float xFactor;
    float yFactor;
    int xCoordinate;
    int yCoordinate;
    int minX;
    int minY;

    bool isMouseClicked;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event) override;
    vector<Point> SeedPoints;

};

#endif // CLICKABLELABEL_H

