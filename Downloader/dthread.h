#ifndef DTHREAD_H
#define DTHREAD_H

#include <QObject>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
//#include "dratemanager.h"

#define DEFBUFSIZE 1024

class DThread : public QObject
{
    Q_OBJECT

public:
    DThread(QObject *parent = 0, qint64 b = 0, qint64 e = 0, long bs = DEFBUFSIZE, QString u = "", QFile *f = 0, bool drm = false);
    void setBufSize(long b);
    void setBegin(qint64 b);
    void setEnd(qint64 e);
    void setFile(QFile *f);
    void setRateManager(bool drm);
    qint64 getBufSize();
    void start(QNetworkAccessManager *nam);

public slots:
    void saveBuf();

signals:
    void signalThreadReadyRead(DThread*);
    void signalThreadFinished(DThread*);
    void signalThreadError(DThread*);

protected:
    void run(QNetworkAccessManager* nam);

private:
    qint64 begin;
    qint64 end;
    qint64 bufSize;
    QString url;
    QFile* file;
    QNetworkReply *reply;
    bool rateManager;

private slots:
    void slotThreadReadyRead();
    void slotThreadFinished();
};

#endif // DTHREAD_H
