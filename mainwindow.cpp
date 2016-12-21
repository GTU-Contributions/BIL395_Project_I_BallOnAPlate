#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectionTh = new ConnectionThread(this);
    connect(connectionTh,SIGNAL(startConnection()),this,SLOT(isConnect()));
    connectionTh->start();

    //getDataTh = new GetDataThread(this);
    //connect(getDataTh,SIGNAL(getDataFromHardware(qreal*,qreal*,qreal*,qreal*),this, SLOT(updateXYCoordinates(qreal,qreal)));
    //getDataTh->start();

    //updateXYCoordinates(20,50);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::isConnect(){

}

/*
void MainWindow::getDataFromHardware(qreal *xP, qreal *yp, qreal *xM, qreal *yM){

}

void MainWindow::updateXYCoordinates(qreal x, qreal y){
    scene = new QGraphicsScene(0,0,150,150);
    ui->xyCoordinates->setScene(scene);

    QPen pen = QPen(Qt::black);
    QBrush brush = QBrush(Qt::black);

    scene->clear();
    scene->addEllipse(x,y,20,20,pen,brush);

}*/

