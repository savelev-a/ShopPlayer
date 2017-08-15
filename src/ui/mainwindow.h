#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "downloadframe.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showDownloadProgress(qint64 current, qint64 total);
    void hideDownloadProgress();

private slots:
    void showWindowByTrayActivation(QSystemTrayIcon::ActivationReason reason);
    void posSliderChangeToPercent(int positionPercent);
    void showSettings();
    void updatePlaylistTable();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    DownloadFrame *downloadFrame;

    QAction *showWindowAction;

    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
