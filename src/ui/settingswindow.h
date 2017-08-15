#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

private slots:
    void settingsChanged();
    void saveSettings();
    void reloadSettings();

private:
    Ui::SettingsWindow *ui;
    QSettings *settings;

};

#endif // SETTINGSWINDOW_H
