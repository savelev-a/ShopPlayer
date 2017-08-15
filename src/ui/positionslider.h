#ifndef POSITIONSLIDER_H
#define POSITIONSLIDER_H

#include <QSlider>
#include <QMouseEvent>

class PositionSlider : public QSlider
{
    Q_OBJECT
public:
    PositionSlider(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // POSITIONSLIDER_H
