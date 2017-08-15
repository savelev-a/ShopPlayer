#ifndef DOWNLOADFRAME_H
#define DOWNLOADFRAME_H

#include <QFrame>
#include <QLabel>
#include <QProgressBar>

class DownloadFrame : public QFrame
{
    Q_OBJECT
public:
    DownloadFrame(QWidget *parent = 0);

public slots:
    void showProgress(qint64 current, qint64 total, const QString &filename);

private:
    QLabel *label;
    QProgressBar *progressBar;

};

#endif // DOWNLOADFRAME_H
