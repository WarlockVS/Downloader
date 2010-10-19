#include "dthread.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>

DThread::DThread(QObject* parent, qint64 b, qint64 e, long bs, QString u, QFile *f, bool drm)
    :QObject (parent), begin(b), end(e), bufSize(bs), url(u), file(f), rateManager(drm)
{
    reply = 0;
}

void DThread::setBufSize(long b)
{
    bufSize = b;
    if(reply) reply->setReadBufferSize(bufSize);
}

void DThread::setEnd(qint64 e)
{
    end = e;
}

void DThread::setBegin(qint64 b)
{
    begin = b;
}

void DThread::setFile(QFile *f)
{
    file = f;
}

void DThread::start(QNetworkAccessManager *nam)
{
    if(url.isEmpty() || !bufSize || !file) return;
    QNetworkRequest request;
    QString byte;
    QString num;

    request.setUrl(QUrl(url));
    if(end)
    {
        byte+= "bytes=";
        byte+= num.setNum(begin);
        byte+= "-";
        byte+=num.setNum(end);
        request.setRawHeader("Range",byte.toLocal8Bit().data()); // задаём размер фрагмента
    }

    reply = nam->get(request);
    if(rateManager) QObject::connect(reply, SIGNAL(readyRead()), SLOT(slotThreadReadyRead()));
    else QObject::connect(reply, SIGNAL(readyRead()), SLOT(saveBuf()));
    QObject::connect(reply, SIGNAL(finished()), SLOT(slotThreadFinished()));
    reply->setReadBufferSize(bufSize);
}

void DThread::slotThreadReadyRead()
{
    if(reply->error())
    {
        qDebug() << "error " << reply->errorString();
        return;
    }
    emit signalThreadReadyRead(this);
}

void DThread::slotThreadFinished()
{
    if(reply->error())
    {
        qDebug() << "error " << reply->errorString();
        return;
    }
    emit signalThreadFinished(this);
    reply->deleteLater();
}

qint64 DThread::getBufSize()
{
    return reply->bytesAvailable();
}

void DThread::saveBuf()
{
    if(reply->error())
    {
        qDebug() << "error " << reply->errorString();
        return;
    }
    file->seek(begin);
    begin+= reply->bytesAvailable();
    file->write(reply->readAll());
    if(begin > end) reply->close();
}

void DThread::setRateManager(bool drm)
{
    if(drm == rateManager) return;
    disconnect(reply,SIGNAL(readyRead()));
    if(drm) connect(reply, SIGNAL(readyRead()), SLOT(slotThreadReadyRead()));
    else connect(reply, SIGNAL(readyRead()), SLOT(saveBuf()));
}
