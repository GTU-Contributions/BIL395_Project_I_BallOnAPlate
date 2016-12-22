#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "drawer.h"

#include <QtCore>
#include "connectionthread.h"
#include "qcustomplot.h"
#include "drawer.h"
#include "scene2d.h"

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

private:
    Ui::MainWindow *ui;
    Scene2d *scene2d;
    Drawer * drawer;
    QMutex mutex;
    QTimer dataTimerFirst;
    QTimer dataTimerSecond;
    QTimer timer2d;
    int xPanel = 500;
    int yPanel = 500;
    int xMotor;
    int yMotor;

    int sentX;
    int sentY;
    char message[64];

public slots:
    void isConnect();

private slots:
    void realtimeDataSlotFirst();
    void realtimeDataSlotSecond();
    void update2DCoordinates();
    void pathDrawer();
    void on_resetBalance_clicked();
    void on_drawCircle_clicked();
    void on_drawSquare_clicked();
    void on_drawRectangle_clicked();
};

#endif // MAINWINDOW_H
