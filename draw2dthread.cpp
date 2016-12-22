#include "draw2dthread.h"

Draw2DThread::Draw2DThread(QObject *parent ):
    QThread(parent)
{
    //
}

void Draw2DThread::run()
{
    emit draw2D();
}
