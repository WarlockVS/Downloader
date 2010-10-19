#include "dratemanager.h"
//#include <QtGlobal>

DRateManager::DRateManager(DRateManager *par, long maxrate)
    :parent(par), maxRate(maxrate), curRate(0)
{
    connect(&t, SIGNAL(timeout()), SLOT(timeOut()));
}

void DRateManager::bufReadyRead(DThread* dt)
{
    toSave.enqueue(dt);
    if(!t.isActive()) t.start(100);
}

void DRateManager::timeOut()
{
    curRate -= maxRate;
    if(curRate < 0) curRate = 0;
    while(curRate < maxRate)
    {
        if(toSave.isEmpty())
        {
            t.stop();
            break;
        }
        DThread* dt;
        dt = toSave.dequeue();
        curRate += dt->getBufSize();
        if(parent) emit bufReady(dt);
        else
        {
            dt->saveBuf();
        }
    }
}

//void DRateManager::threadFinished()
//{

//}
