#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include <QString>
#include <QDateTime>
#include <QFileInfo>
#include <QMediaContent>

class AudioClip
{
public:

    enum ClipType
    {
        MUSIC = 0,
        REKLAMA = 1
    };

    AudioClip(QFileInfo file);
    AudioClip(const QString &filename);

    ClipType type;

    QString getName() const;
    QString getFullPath() const;

    QDateTime startDateTime;
    QDateTime endDateTime;

    QMediaContent getContent() const;
    bool isValid();

    QString toString();

private:
    QFileInfo file;
};

#endif // AUDIOCLIP_H
