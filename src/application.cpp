#include "application.h"

#include <QDebug>

Application* Application::appInstance = 0;

Application* Application::getInstance()
{
    if(appInstance == 0) appInstance = new Application;

    return appInstance;
}

void Application::init()
{
    qDebug() << "Загрузка настроек..";
    settings = new QSettings("settings.ini", QSettings::IniFormat, this);

    qDebug() << "Запуск сервисов..";
    mediaPlayer = new MediaPlayer(this);
    playlistModel = new PlaylistModel(this);
    networkClient = new NetworkClient(this);
    mainWindow = new MainWindow();

    QTimer::singleShot(1000, networkClient, SLOT(checkAndDownloadData()));




    qDebug() << "Загрузка завершена";
}
