/*#ifndef GETDATATHREAD_H
#define GETDATATHREAD_H

#include <QThread>

class GetDataThread : public QThread
{
    Q_OBJECT
public:
    explicit GetDataThread(QObject *parent = 0);
    void run();
signals:
    void updateXYCoordinate(qreal x, qreal y);
public slots:

};

#endif // GETDATATHREAD_H*/
