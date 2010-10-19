#ifndef QDTHREAD_H
#define QDTHREAD_H

//#include <QtGlobal>
#include <QObject>
#include <QTimer>
#include <QNetworkReply>
#include <QAuthenticator>

class QDThread : public QObject
{

    Q_OBJECT

public:
    QDThread(qint64 s = 0, qint64 e = 0, qint64 b = 0);
    void setBufSize(qint64 bs);
    void start(QNetworkAccessManager* man, QUrl url);

public slots:
    void readBuffer();
    void sheduleReadBuffer();

protected:
    void run();

private:
    qint64 Start;
    qint64 End;
    qint64 BufSize;
    QTimer  stopWatch;
    bool readBuferSheduled;
    QNetworkReply* currThread;
    QNetworkAccessManager* manager;
};

#endif // QDTHREAD_H
