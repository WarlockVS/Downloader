#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QFile>
#include <QAuthenticator>
//#include <QFuture>
#include "dthread.h"

class Download : public QObject
{
    Q_OBJECT
public:
    explicit Download(QObject *parent = 0);
    ~Download();
    void setUrl(QString u);
    void setFileName(QString fn);
    void setMaxRate(int mr);
    void setMaxThreads(int mt);
    void start();

signals:
    void downloadFinished(Download*);

public slots:
//    void savingResult(int index);
//    void slotFinished();

private:
    QString url;
    int maxRate;
    int maxThreads;
    QString fileName;
    QNetworkAccessManager* manager;
    QNetworkReply* reply;
//    QFutureWatcher fileSaving;
    qint64 length;
    QFile file;
    QList<DThread*> sections;

private slots:
    void fileReplyFinished();
    void httpAuthenticationRequired(QNetworkReply *,  QAuthenticator*);
    void slotThreadFinished(DThread*);
};

#endif // DOWNLOAD_H
