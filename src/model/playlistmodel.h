#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "audioclip.h"

class PlaylistModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    PlaylistModel(QObject *parent = 0);
    ~PlaylistModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void loadFromJson();

signals:
    void loadComplete(const QList<AudioClip> &cliplist);

private:
    QList<AudioClip> clipList;
};

#endif // PLAYLISTMODEL_H
