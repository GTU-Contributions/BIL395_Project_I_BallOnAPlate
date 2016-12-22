#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "drawer.h"

#include <QtCore>
#include "connectionthread.h"
#include "qcustomplot.h"
#include "drawer.h"
#include "draw2dthread.h"

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
    Draw2DThread *drawingTh;


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene ;
    QMutex mutex;
    QTimer dataTimerFirst;
    QTimer dataTimerSecond;
    Drawer * drawer;
    int xPanel;
    int yPanel;
    int xMotor;
    int yMotor;
    int sentX;
    int sentY;


public slots:
    void isConnect();
    void update2DCoordinates();


private slots:
    void realtimeDataSlotFirst();
    void realtimeDataSlotSecond();
};

#endif // MAINWINDOW_H
