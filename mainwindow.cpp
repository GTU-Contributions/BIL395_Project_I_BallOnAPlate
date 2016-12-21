#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    drawer = new Drawer(this);
    ui->shapeDrawer->setScene(drawer);
    ui->shapeDrawer->setRenderHint(QPainter::Antialiasing);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}
