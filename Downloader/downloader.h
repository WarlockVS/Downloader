#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QFile>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QProgressBar>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);

    void append(const QUrl &url);
    void append(const QStringList &urlList);
    QString saveFileName(const QUrl &url);

signals:
    void finished();

private slots:
    void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();
//    void httpRequestFinished(int requestId, bool error);

private:
    QNetworkAccessManager manager;
    QQueue<QUrl> downloadQueue;
    QNetworkReply *currentDownload;
    QFile output;
    QTime downloadTime;
    QProgressBar progressBar;

    int downloadedCount;
    int totalCount;

public slots:

};

#endif // DOWNLOADER_H
