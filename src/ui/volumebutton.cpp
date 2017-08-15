#include "volumebutton.h"

#include <QHBoxLayout>
#include <QWidgetAction>
#include <QMenu>

#include "../application.h"

VolumeButton::VolumeButton(QWidget *parent) : QToolButton(parent)
{
    setPopupMode(QToolButton::InstantPopup);
    setArrowType(Qt::NoArrow);

    QWidget *popupWidget = new QWidget(this);

    volumeSlider = new QSlider(Qt::Horizontal, popupWidget);
    volumeSlider->setRange(0, 100);

    QHBoxLayout *popupLayout = new QHBoxLayout(popupWidget);
    popupLayout->setMargin(2);
    popupLayout->addWidget(volumeSlider);

    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(popupWidget);

    QMenu *menu = new QMenu(this);
    menu->addAction(action);
    setMenu(menu);

    int savedVolume = Application::getInstance()->settings->value("volume", 50).toInt();
    volumeSlider->setValue(savedVolume);
    changeVolume(savedVolume);

    connect(this->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeVolume(int)));
    connect(this->volumeSlider, SIGNAL(sliderReleased()), this, SLOT(saveVolumeToSettings()));
}

void VolumeButton::changeVolume(int volume)
{
    if(volume == 0) this->setIcon(QIcon(":/res/img/32x32/volume-muted.png"));
    else if(volume < 33) this->setIcon(QIcon(":/res/img/32x32/volume-low.png"));
    else if(volume < 66) this->setIcon(QIcon(":/res/img/32x32/volume-medium.png"));
    else this->setIcon(QIcon(":/res/img/32x32/volume-high.png"));

    Application::getInstance()->mediaPlayer->setVolume(volume);

}

void VolumeButton::saveVolumeToSettings()
{
    Application::getInstance()->settings->setValue("volume", volumeSlider->value());
}
