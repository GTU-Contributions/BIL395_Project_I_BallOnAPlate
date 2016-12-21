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

    //Mutlu Polatcan
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40,110,255)));
    ui->customPlot->graph(1)->setPen(QPen(QColor(255,110,40)));
    ui->customPlotSecond->addGraph(); // blue line
    ui->customPlotSecond->addGraph(); // red line
    ui->customPlotSecond->graph(0)->setPen(QPen(QColor(40,110,255)));
    ui->customPlotSecond->graph(1)->setPen(QPen(QColor(255,110,40)));

    QSharedPointer<QCPAxisTickerTime> timeTickerFirst(new QCPAxisTickerTime);
    QSharedPointer<QCPAxisTickerTime> timeTickerSecond(new QCPAxisTickerTime);
    timeTickerFirst->setTimeFormat("%h:%m:%s");
    timeTickerSecond->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTickerFirst);
    ui->customPlotSecond->xAxis->setTicker(timeTickerSecond);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlotSecond->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(0,1023);
    ui->customPlotSecond->yAxis->setRange(0,767);

    connect(ui->customPlot->xAxis,SIGNAL(rangeChanged(QCPRange)),ui->customPlot->xAxis2,SLOT(setRange(QCPRange)));
    connect(ui->customPlotSecond->xAxis,SIGNAL(rangeChanged(QCPRange)),ui->customPlotSecond->xAxis2,SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis,SIGNAL(rangeChanged(QCPRange)),ui->customPlot->yAxis2,SLOT(setRange(QCPRange)));
    connect(ui->customPlotSecond->yAxis,SIGNAL(rangeChanged(QCPRange)),ui->customPlotSecond->yAxis2,SLOT(setRange(QCPRange)));
    connect(&dataTimerFirst,SIGNAL(timeout()),this,SLOT(realtimeDataSlotFirst()));
    connect(&dataTimerSecond,SIGNAL(timeout()),this,SLOT(realtimeDataSlotSecond()));
    dataTimerFirst.start();
    dataTimerSecond.start();
    //Mutlu Polatcan
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::isConnect(){

}

void MainWindow::realtimeDataSlotFirst() {
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key-lastPointKey > 0.5) // at most add point every 2 ms
    {
      // add data to lines:
      ui->customPlot->graph(0)->addData(key, rand() % 1023);
  //    ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->customPlot->graph(0)->data()->size()/*ui->customPlot->graph(1)->data()->size()*/)
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}

void MainWindow::realtimeDataSlotSecond() {
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key-lastPointKey > 0.5) // at most add point every 2 ms
    {
      // add data to lines:
      ui->customPlotSecond->graph(0)->addData(key, rand()% 767);
  //    ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlotSecond->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlotSecond->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->customPlotSecond->graph(0)->data()->size()/*ui->customPlot->graph(1)->data()->size()*/)
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}

