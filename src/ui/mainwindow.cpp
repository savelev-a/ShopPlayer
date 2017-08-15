#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingswindow.h"
#include "../application.h"

#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(640, 480);

    trayMenu = new QMenu(this);
    trayIcon = new QSystemTrayIcon(QIcon(":/res/img/16x16/appicon.png"), this);
    trayIcon->setContextMenu(trayMenu);

    showWindowAction = new QAction("Открыть плеер", this);

    trayMenu->addAction(showWindowAction);
    trayMenu->addAction(ui->loadAllAction);
    trayMenu->addSeparator();
    trayMenu->addAction(ui->sBackwardAction);
    trayMenu->addAction(ui->backwardAction);
    trayMenu->addAction(ui->playAction);
    trayMenu->addAction(ui->pauseAction);
    trayMenu->addAction(ui->stopAction);
    trayMenu->addAction(ui->forwardAction);
    trayMenu->addAction(ui->sForwardAction);
    trayMenu->addSeparator();
    trayMenu->addAction(ui->exitAction);

    downloadFrame = new DownloadFrame(this);
    downloadFrame->move(this->width() / 2 - downloadFrame->width() / 2,
                        this->height() / 2 - downloadFrame->height() / 2);
    downloadFrame->setVisible(false);


    MediaPlayer *mplayer = Application::getInstance()->mediaPlayer;
    PlaylistModel *model = Application::getInstance()->playlistModel;
    NetworkClient *netClient = Application::getInstance()->networkClient;

    connect(model, SIGNAL(loadComplete(QList<AudioClip>)), mplayer, SLOT(loadPlaylist(QList<AudioClip>)));
    connect(model, SIGNAL(loadComplete(QList<AudioClip>)), this, SLOT(updatePlaylistTable()));

    ui->volumeButton->changeVolume(Application::getInstance()->settings->value("volume", 50).toInt());

    connect(ui->exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    connect(showWindowAction, SIGNAL(triggered(bool)), this, SLOT(show()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(showWindowByTrayActivation(QSystemTrayIcon::ActivationReason)));

    connect(ui->settingsAction, SIGNAL(triggered(bool)), this, SLOT(showSettings()));
    connect(ui->loadAllAction, SIGNAL(triggered(bool)), netClient, SLOT(checkAndDownloadData()));
    connect(ui->parseLocalFileAction, SIGNAL(triggered(bool)), model, SLOT(loadFromJson()));

    connect(ui->mediaPlayButton, SIGNAL(clicked(bool)), mplayer, SLOT(play()));
    connect(ui->mediaStopButton, SIGNAL(clicked(bool)), mplayer, SLOT(stop()));
    connect(ui->mediaPauseButton, SIGNAL(clicked(bool)), mplayer, SLOT(pause()));
    connect(ui->mediaBackwardButton, SIGNAL(clicked(bool)), mplayer, SLOT(rewind5secs()));
    connect(ui->mediaForwardButton, SIGNAL(clicked(bool)), mplayer, SLOT(forward5secs()));
    connect(ui->mediaSBackwardButton, SIGNAL(clicked(bool)), mplayer, SLOT(previousTrack()));
    connect(ui->mediaSForwardButton, SIGNAL(clicked(bool)), mplayer, SLOT(nextTrack()));

    connect(ui->playAction, SIGNAL(triggered(bool)), mplayer, SLOT(play()));
    connect(ui->stopAction, SIGNAL(triggered(bool)), mplayer, SLOT(stop()));
    connect(ui->pauseAction, SIGNAL(triggered(bool)), mplayer, SLOT(pause()));
    connect(ui->backwardAction, SIGNAL(triggered(bool)), mplayer, SLOT(rewind5secs()));
    connect(ui->forwardAction, SIGNAL(triggered(bool)), mplayer, SLOT(forward5secs()));
    connect(ui->sBackwardAction, SIGNAL(triggered(bool)), mplayer, SLOT(previousTrack()));
    connect(ui->sForwardAction, SIGNAL(triggered(bool)), mplayer, SLOT(nextTrack()));

    connect(mplayer, SIGNAL(posChangedInPercent(int)), this, SLOT(posSliderChangeToPercent(int)));
    connect(ui->positionSlider, SIGNAL(valueChanged(int)), mplayer, SLOT(posChangeToPercent(int)));

    connect(ui->localPlaylistTable, SIGNAL(clicked(QModelIndex)), mplayer, SLOT(playSelectedTrack(QModelIndex)));

    trayIcon->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    setVisible(false);
    event->ignore();
}

void MainWindow::showWindowByTrayActivation(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick) this->setVisible(true);
}

void MainWindow::posSliderChangeToPercent(int positionPercent)
{
    ui->positionSlider->blockSignals(true);
    ui->positionSlider->setValue(positionPercent);
    ui->positionSlider->blockSignals(false);

    qint64 msPosition = Application::getInstance()->mediaPlayer->position();
    qint64 msDuration = Application::getInstance()->mediaPlayer->duration();

    int minutes = msPosition / 1000 / 60;
    int seconds = (msPosition / 1000) % 60;

    QString minutesStr = minutes > 9 ? QString::number(minutes) : "0" + QString::number(minutes);
    QString secondsStr = seconds > 9 ? QString::number(seconds) : "0" + QString::number(seconds);

    if(minutes == 0 && seconds == 0)
    {
        int minutesDuration = msDuration / 1000 / 60;
        int secondsDuration = (msDuration / 1000) % 60;

        QString minutesDurationStr = minutesDuration > 9
                ? QString::number(minutesDuration)
                : "0" + QString::number(minutesDuration);
        QString secondsDurationStr = secondsDuration > 9
                ? QString::number(secondsDuration)
                : "0" + QString::number(secondsDuration);

        ui->durationLabel->setText(minutesDurationStr + ":" + secondsDurationStr);

        ui->localPlaylistTable->selectRow(Application::getInstance()->mediaPlayer->localPlaylist->currentIndex());
    }



    ui->timeLabel->setText(minutesStr + ":" + secondsStr);

}



void MainWindow::showSettings()
{
    SettingsWindow *settingsWindow = new SettingsWindow();
    settingsWindow->setVisible(true);
}

void MainWindow::showDownloadProgress(qint64 current, qint64 total)
{
    downloadFrame->showProgress(current, total, Application::getInstance()->networkClient->getCurrentFilename());
}

void MainWindow::hideDownloadProgress()
{
    downloadFrame->setVisible(false);
}

void MainWindow::updatePlaylistTable()
{
    ui->localPlaylistTable->setModel(Application::getInstance()->playlistModel);
    ui->localPlaylistTable->setColumnHidden(1, true);
    ui->localPlaylistTable->horizontalHeader()->stretchLastSection();
    ui->localPlaylistTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}
