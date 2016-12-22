#ifndef DRAW2DTHREAD_H
#define DRAW2DTHREAD_H

#include <QtCore>
#include <QThread>

class Draw2DThread : public QThread
{
    Q_OBJECT
public:
    explicit Draw2DThread(QObject *parent = 0);
    void run();

signals:
    void draw2D();
};

#endif // DRAW2DTHREAD_H
