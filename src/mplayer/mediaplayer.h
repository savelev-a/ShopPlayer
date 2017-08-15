#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QModelIndex>

#include "../model/audioclip.h"

class MediaPlayer : public QMediaPlayer
{
    Q_OBJECT
public:
    explicit MediaPlayer(QObject *parent = 0);

    QMediaPlaylist *localPlaylist;

signals:
    void posChangedInPercent(int positionPercent);

public slots:
    void loadPlaylist(const QList<AudioClip> &clipList);
    void posChangeToPercent(int positionPercent);

    void rewind5secs();
    void forward5secs();
    void nextTrack();
    void previousTrack();

    void playSelectedTrack(QModelIndex tableIdx);

private slots:
    void posChangedConverter(qint64 posInMillisecs);
    void refreshCounter();

private:
    int counter;
};

#endif // MEDIAPLAYER_H
