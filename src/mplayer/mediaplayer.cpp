#include "mediaplayer.h"

#include <QFile>
#include <QDir>

#include "../application.h"

#include <QDebug>

MediaPlayer::MediaPlayer(QObject *parent) : QMediaPlayer(parent)
{
    localPlaylist = new QMediaPlaylist(this);
    localPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    this->setPlaylist(localPlaylist);

    this->counter = 0;

    connect(this, SIGNAL(positionChanged(qint64)), this, SLOT(posChangedConverter(qint64)));
    connect(this->playlist(), SIGNAL(currentIndexChanged(int)), this, SLOT(refreshCounter()));
}

void MediaPlayer::loadPlaylist(const QList<AudioClip> &clipList)
{
    localPlaylist->clear();
    for(AudioClip clip : clipList)
    {
        localPlaylist->addMedia(clip.getContent());
    }

    this->play();
}

void MediaPlayer::posChangeToPercent(int positionPercent)
{
    this->setPosition((positionPercent * this->duration()) / 100);
}

void MediaPlayer::posChangedConverter(qint64 posInMillisecs)
{
    int currentDuration = this->duration() > 0 ? this->duration() : 1;
    emit posChangedInPercent((posInMillisecs * 100) / currentDuration);
}

void MediaPlayer::rewind5secs()
{
    this->setPosition(this->position() <= 5000 ? 0 : this->position() - 5000);
}

void MediaPlayer::forward5secs()
{
    this->setPosition(this->position() + 5000 >= this->duration() ? this->duration() : this->position() + 5000);
}

void MediaPlayer::nextTrack()
{
    localPlaylist->next();
}

void MediaPlayer::previousTrack()
{
    localPlaylist->previous();
}

void MediaPlayer::playSelectedTrack(QModelIndex tableIdx)
{
    this->localPlaylist->setCurrentIndex(tableIdx.row());
}

void MediaPlayer::refreshCounter()
{
    int interval = Application::getInstance()->settings->value("periodCheck", 30).toInt();
    if(counter >= interval)
    {
        Application::getInstance()->networkClient->checkAndDownloadData();
        counter = 0;
    }

    counter++;
}
