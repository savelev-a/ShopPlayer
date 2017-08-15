#ifndef VOLUMEBUTTON_H
#define VOLUMEBUTTON_H

#include <QToolButton>
#include <QSlider>

class VolumeButton : public QToolButton
{
    Q_OBJECT
public:
    VolumeButton(QWidget *parent = 0);

    QSlider *volumeSlider;

public slots:
    void changeVolume(int volume);

private slots:
    void saveVolumeToSettings();
};

#endif // VOLUMEBUTTON_H
