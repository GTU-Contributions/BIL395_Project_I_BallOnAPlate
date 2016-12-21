#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updateXYCoordinates(20,50);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateXYCoordinates(qreal x, qreal y){
    scene = new QGraphicsScene(0,0,150,150);
    ui->xyCoordinates->setScene(scene);

    QPen pen = QPen(Qt::black);
    QBrush brush = QBrush(Qt::black);

    scene->clear();
    scene->addEllipse(x,y,20,20,pen,brush);

}
