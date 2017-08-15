#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QSettings>
#include <QTimer>
#include "ui/mainwindow.h"
#include "mplayer/mediaplayer.h"
#include "model/playlistmodel.h"
#include "network/networkclient.h"

class Application : public QObject
{
    Q_OBJECT
public:
    static Application* getInstance();

    void init();

    QSettings *settings;
    MediaPlayer *mediaPlayer;
    PlaylistModel *playlistModel;
    NetworkClient *networkClient;
    MainWindow *mainWindow;
    //QTimer *refreshTimer;

private:
    static Application *appInstance;

};

#endif // APPLICATION_H
