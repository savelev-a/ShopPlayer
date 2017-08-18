#include "networkclient.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QTextCodec>
#include <QDir>

#include "../application.h"

NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    if(!QDir("local").exists())
    {
        qDebug() << "Создание каталогов..";
        QDir().mkdir("local");
    }

    connect(this, SIGNAL(downloadComplete()), Application::getInstance()->playlistModel, SLOT(loadFromJson()));
}


void NetworkClient::downloadFile(const QString &filename)
{
    QSettings *settings = Application::getInstance()->settings;
    QString ftpAddress = settings->value("ftpAddress").toString();
    int ftpPort = settings->value("ftpPort", 21).toInt();
    QString username = settings->value("ftpUsername").toString();
    QString password = settings->value("ftpPassword").toString();

    currentFilename = filename;
    QFile currentFile("local/" + this->currentFilename);
    if(currentFile.exists()) currentFile.remove();

    QString address = "ftp://" + ftpAddress + ":" + QString::number(ftpPort) + "/" + filename;

    QUrl url(address);
    url.setUserName(username);
    url.setPassword(password);
    QNetworkRequest request(url);

    //qDebug() << url.toString();

    currentReply = manager->get(request);

    connect(currentReply, SIGNAL(readyRead()), this, SLOT(downloadBytesAvaible()));
    connect(currentReply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(currentReply, SIGNAL(finished()), Application::getInstance()->mainWindow, SLOT(hideDownloadProgress()));
    connect(currentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(showError(QNetworkReply::NetworkError)));
    connect(currentReply, SIGNAL(downloadProgress(qint64,qint64)),
            Application::getInstance()->mainWindow, SLOT(showDownloadProgress(qint64,qint64)));

}

void NetworkClient::downloadBytesAvaible()
{
    QFile file("local/" + this->currentFilename);
    if(file.open(QFile::ReadWrite | QFile::Append))
    {
        file.write(currentReply->readAll());
        file.close();
    }
    else
    {
        showError("Ошибка записи в файл " + currentFilename + ": " + file.errorString());
    }
}

void NetworkClient::downloadFinished()
{
    if(currentFilename == "content.json")
    {
        this->parseContentJson();
    }
    else
    {
        if(!filenamesToDownload.isEmpty())
        {
            downloadFile(filenamesToDownload.first());
            filenamesToDownload.removeFirst();
        }
        else
        {
            emit downloadComplete();
        }
    }
}

void NetworkClient::showError(const QString &errorString)
{
    QMessageBox::warning(0, "Ошибка загрузки файла", errorString);
}

void NetworkClient::showError(QNetworkReply::NetworkError error)
{
    showError(currentReply->errorString());
}


void NetworkClient::checkAndDownloadData()
{
    downloadFile("content.json");
}

void NetworkClient::parseContentJson()
{
    QString shopname = Application::getInstance()->settings->value("shopname").toString();
    filenamesToDownload.clear();

    QFile contentFile("local/content.json");
    if(contentFile.exists() && contentFile.open(QFile::ReadOnly))
    {
        QJsonDocument document = QJsonDocument::fromJson(contentFile.readAll());
        QJsonArray shopsArray = document.array();
        for(QJsonValue shopVal : shopsArray)
        {
            QJsonObject shop = shopVal.toObject();
            if(shop.value("shopname").toString() == shopname || shop.value("shopname").toString() == "ALL")
            {
                QJsonArray contentArray = shop.value("content").toArray();
                for(QJsonValue clipVal : contentArray)
                {
                    QJsonObject clipFromJson = clipVal.toObject();
                    QString mediaRelPath = clipFromJson.value("filename").toString().prepend("local/");
                    QFileInfo file(mediaRelPath);
                    if(!file.exists())
                    {
                        filenamesToDownload.append((clipFromJson.value("filename").toString()));
                    }
                }
            }
        }
    }

    if(!filenamesToDownload.isEmpty())
    {
        downloadFile(filenamesToDownload.first());
        filenamesToDownload.removeFirst();
    }
    else
    {
        emit downloadComplete();
    }
}

QString NetworkClient::getCurrentFilename() const
{
    return currentFilename;
}
