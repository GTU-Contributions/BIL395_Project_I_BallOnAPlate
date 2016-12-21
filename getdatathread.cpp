/*#include "getdatathread.h"

#include <QtCore>

GetDataThread::GetDataThread(QObject *parent) : QThread(parent)
{

}

void GetDataThread::run(){
    QMutex mutex;
    mutex.lock();

    mutex.unlock();

    emit getDataFromHardware(qreal *xP, qreal *yp, qreal *xM, qreal *yM);

}
*/
