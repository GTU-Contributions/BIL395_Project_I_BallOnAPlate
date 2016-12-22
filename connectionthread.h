#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtCore>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include "rs232.h"

class ConnectionThread : public QThread
{
    Q_OBJECT
public:
    explicit ConnectionThread(QObject *parent = 0);
    void run();
private:
    QMutex mutex;
signals:
    void startConnection();
public slots:

};

#endif // CONNECTIONTHREAD_H
