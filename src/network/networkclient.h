#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetworkClient : public QObject
{
    Q_OBJECT
public:
    explicit NetworkClient(QObject *parent = 0);
    QString getCurrentFilename() const;

public slots:
    void checkAndDownloadData();

signals:
    void downloadComplete();

private slots:
    void downloadBytesAvaible();
    void downloadFinished();
    void showError(QNetworkReply::NetworkError error);
    void showError(const QString &errorString);

private:
    QNetworkAccessManager *manager;
    QString currentFilename;
    QNetworkReply *currentReply;
    QStringList filenamesToDownload;
    void downloadFile(const QString &filename);
    void parseContentJson();
};

#endif // NETWORKCLIENT_H
