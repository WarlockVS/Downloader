#include "qdthread.h"

#define READINTERVAL    100

QDThread::QDThread(qint64 s, qint64 e, qint64 b):
                   Start(s), End(e), BufSize(b)
{
    readBuferSheduled = false;
}

void QDThread::sheduleReadBuffer()
{
    if(readBuferSheduled) return;
    readBuferSheduled = true;
    QTimer::singleShot(READINTERVAL, this, SLOT(readBuffer()));
}

void QDThread::readBuffer()
{
//    int msec = 1000;
//    readBuferSheduled = false;
//    if(stopWatch.isActive()) msec = qMin(msec, stopWatch.elapsed());
//    qint64 bytesToRead = (MaxRate * msec) / 1000;
//    if(!bytesToRead){shedileReabBuffer(); return;}
//    currThread->bytesAvailable();
//    stopWatch.start();
}

//void QDThread::setMaxRate(qint64 mr)
//{
//    MaxRate = mr;
//    currThread->setReadBufferSize(MaxRate * READINTERVAL / 1000);
//}

void QDThread::run()
{
    connect(currThread, SIGNAL(readyRead()),SLOT(readBuffer()));
}
