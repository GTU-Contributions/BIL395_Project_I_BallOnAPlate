#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "drawer.h"

#include <QtCore>
#include "connectionthread.h"
#include "qcustomplot.h"
#include "drawer.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ConnectionThread *connectionTh;
    //GetDataThread *getDataTh;

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene ;
    QTimer dataTimerFirst;
    QTimer dataTimerSecond;
    Drawer * drawer;
    qreal xCoor;
    qreal yCoor;

public slots:
    void isConnect();
   //void updateXYCoordinates(qreal x, qreal y);

private slots:
    void realtimeDataSlotFirst();
    void realtimeDataSlotSecond();
    Drawer * drawer;
};

#endif // MAINWINDOW_H
