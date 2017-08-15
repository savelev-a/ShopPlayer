#include "playlistmodel.h"

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

#include "../application.h"

#include <QDebug>

PlaylistModel::PlaylistModel(QObject *parent) :QAbstractTableModel(parent)
{

}

PlaylistModel::~PlaylistModel(){}


int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return clipList.size();
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    /* 0 - индекс
     * 1 - полный путь
     * 2 - дата начала
     * 3 - дата окончания
     * 4 - тип
     * 5 - имя файла
     * (6 - длительность (резерв))
    */

    return 6;
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            return QVariant(index.row() + 1);
            break;

        case 1:
            return QVariant(clipList.at(index.row()).getFullPath());
            break;

        case 2:
            if(clipList.at(index.row()).type == AudioClip::REKLAMA)
                return QVariant(clipList.at(index.row()).startDateTime.toString("dd.MM.yyyy hh:mm:ss"));
            else
                return QVariant("");
            break;

        case 3:
            if(clipList.at(index.row()).type == AudioClip::REKLAMA)
                return QVariant(clipList.at(index.row()).endDateTime.toString("dd.MM.yyyy hh:mm:ss"));
            else
                return QVariant("");
            break;

        case 4:
            return QVariant(clipList.at(index.row()).type == AudioClip::MUSIC ? "Музыка" : "Реклама");
            break;

        case 5:
            return QVariant(clipList.at(index.row()).getName());
            break;
        }

        break; //Qt::DisplayRole

    }

    return QVariant();
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical) return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        switch (section) {
        case 0:
            return QVariant("#");
            break;
        case 1:
            return QVariant("FilePath");
            break;
        case 2:
            return QVariant("Дата начала");
            break;
        case 3:
            return QVariant("Дата окончания");
            break;
        case 4:
            return QVariant("Тип");
            break;
        case 5:
            return QVariant("Название ролика");
            break;
        }
        break;

    } //Qt::DisplayRole

    return QVariant();
}

void PlaylistModel::loadFromJson()
{
    QString shopname = Application::getInstance()->settings->value("shopname").toString();

    QList<AudioClip> musicList;
    QList<AudioClip> reklamaList;

    clipList.clear();

    QFile contentFile("local/content.json");
    if(contentFile.exists() && contentFile.open(QFile::ReadOnly))
    {
        QJsonDocument document = QJsonDocument::fromJson(contentFile.readAll());
        QJsonArray shopsArray = document.array();
        for(QJsonValue shopVal : shopsArray)
        {
            QJsonObject shop = shopVal.toObject();
            //qDebug() << shop.value("shopname").toString() << shopname;
            if(shop.value("shopname").toString() == shopname)
            {
                QJsonArray contentArray = shop.value("content").toArray();
                for(QJsonValue clipVal : contentArray)
                {
                    QJsonObject clipFromJson = clipVal.toObject();
                    QString mediaRelPath = clipFromJson.value("filename").toString().prepend("local/");
                    QFileInfo file(mediaRelPath);

                    AudioClip clipToModel(file);
                    clipToModel.type = static_cast<AudioClip::ClipType>(clipFromJson.value("type").toInt());
                    if(clipToModel.type == AudioClip::REKLAMA)
                    {
                        clipToModel.startDateTime = QDateTime::fromString(
                                    clipFromJson.value("start").toString(), "dd.MM.yyyy hh:mm:ss");
                        clipToModel.endDateTime = QDateTime::fromString(
                                    clipFromJson.value("end").toString(), "dd.MM.yyyy hh:mm:ss");
                        if(clipToModel.isValid()) reklamaList.append(clipToModel);
                    }
                    else
                    {
                        if(clipToModel.isValid()) musicList.append(clipToModel);
                    }
                }
            }
        }

        int currentReklamaPos = 0;
        for(AudioClip musicClip : musicList)
        {
            clipList.append(musicClip);
            if(!reklamaList.isEmpty() && currentReklamaPos <= reklamaList.size() - 1)
            {
                clipList.append(reklamaList.at(currentReklamaPos));
            }

            currentReklamaPos++;
            if(currentReklamaPos > reklamaList.size() - 1) currentReklamaPos = 0;
        }

        emit loadComplete(this->clipList);

    }
}
