#include "positionslider.h"

PositionSlider::PositionSlider(QWidget *parent) : QSlider(Qt::Horizontal, parent)
{

}


void PositionSlider::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setValue(minimum() + ((maximum()-minimum()) * event->x()) / width());
        event->accept();
    }

    QSlider::mousePressEvent(event);
}
