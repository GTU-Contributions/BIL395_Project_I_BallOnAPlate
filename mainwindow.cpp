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
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::isConnect(){

}


