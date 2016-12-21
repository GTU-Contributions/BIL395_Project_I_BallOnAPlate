#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include <QtCore>
#include "connectionthread.h"

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
    qreal xCoor;
    qreal yCoor;

public slots:
    void isConnect();
   //void updateXYCoordinates(qreal x, qreal y);
};

#endif // MAINWINDOW_H
