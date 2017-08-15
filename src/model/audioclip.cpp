#include "audioclip.h"

AudioClip::AudioClip(QFileInfo file)
{
    this->file = file;
}

AudioClip::AudioClip(const QString &filename)
{
    this->file = QFileInfo(filename);
}

QString AudioClip::getName() const
{
    return file.fileName();
}

QString AudioClip::getFullPath() const
{
    return file.absoluteFilePath();
}

QMediaContent AudioClip::getContent() const
{
    return QMediaContent(QUrl::fromLocalFile(this->getFullPath()));
}

bool AudioClip::isValid()
{
    switch (this->type) {
    case MUSIC:
        return QFile(this->getFullPath()).exists();
        break;
    case REKLAMA:
        return (QFile(this->getFullPath()).exists()
                && this->startDateTime <= QDateTime::currentDateTime()
                && this->endDateTime >= QDateTime::currentDateTime());
        break;
    default:
        return false;
        break;
    }
}

QString AudioClip::toString()
{
    QString res = "AudioClip {filename: " + getName()
            + ", path: " + getFullPath()
            + ", type: " + type
            + ", start: " + startDateTime.toString("dd.MM.yyyy hh:mm:ss")
            + ", stop: " + endDateTime.toString("dd.MM.yyyy hh:mm:s")
            + "}";

    return res;

}
