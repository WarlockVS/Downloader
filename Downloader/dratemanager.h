#ifndef DRATEMANAGER_H
#define DRATEMANAGER_H

#include <QQueue>
#include <QTimer>
#include "dthread.h"

class DRateManager : public QObject
{
     Q_OBJECT

public:
    DRateManager(DRateManager* par = 0, long maxrate = 0);

signals:
    void bufReady(DThread* dt);

public slots:
    void bufReadyRead(DThread* dt);
//    void threadFinished();
    void timeOut();

private:
    QTimer t;
    DRateManager* parent;
    QQueue<DThread*> toSave;
    long maxRate;
    long curRate;
};

#endif // DRATEMANAGER_H
