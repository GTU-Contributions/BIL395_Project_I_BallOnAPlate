
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    scene = new QGraphicsScene(0,0,150,150);
    ui->xyCoordinates->setScene(scene);

    connectionTh = new ConnectionThread(this);
    drawingTh = new Draw2DThread(this);
    connect(connectionTh, SIGNAL(startConnection()), this, SLOT(isConnect()), Qt::DirectConnection);
    connect(drawingTh,SIGNAL(draw2D()),this,SLOT(update2DCoordinates()), Qt::DirectConnection);

    //Kaan Uçar
    drawer = new Drawer(this);
    ui->shapeDrawer->setScene(drawer);
    ui->shapeDrawer->setRenderHint(QPainter::Antialiasing);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    //Kaan Uçar

    //Mutlu Polatcan
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->customPlotSecond->addGraph(); // blue line
    ui->customPlotSecond->addGraph(); // red line
    ui->customPlotSecond->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlotSecond->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer <QCPAxisTickerTime> timeTickerFirst(new QCPAxisTickerTime);
    QSharedPointer <QCPAxisTickerTime> timeTickerSecond(new QCPAxisTickerTime);
    timeTickerFirst->setTimeFormat("%h:%m:%s");
    timeTickerSecond->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTickerFirst);
    ui->customPlotSecond->xAxis->setTicker(timeTickerSecond);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlotSecond->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(0, 1023);
    ui->customPlotSecond->yAxis->setRange(0, 800);

    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlotSecond->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlotSecond->xAxis2,
            SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlotSecond->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlotSecond->yAxis2,
            SLOT(setRange(QCPRange)));
    connect(&dataTimerFirst, SIGNAL(timeout()), this, SLOT(realtimeDataSlotFirst()));
    connect(&dataTimerSecond, SIGNAL(timeout()), this, SLOT(realtimeDataSlotSecond()));
    dataTimerFirst.start();
    dataTimerSecond.start();
    //Mutlu Polatcan
    qDebug("Running GUI thread");
    connectionTh->start();
    drawingTh->start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::update2DCoordinates(){
    QPen pen = QPen(Qt::black);
    QBrush brush = QBrush(Qt::black);

    qDebug("geldim çizerim");
    //mutex.lock();
    scene->addEllipse(xMotor,yMotor,20,20,pen,brush);
    //mutex.unlock();



        scene->clear();


}


void MainWindow::isConnect() {
    int i = 0,
            n,
            x = 0,
            y = 0,
            cport_nr = 24,        /* /dev/ttyS0 (COM1 on windows) */
            bdrate = 115200;       /* 115200 baud */

    char mode[] = {'8', 'N', '1', 0},
            str[64];

    unsigned char buf[2];

    mutex.lock();
    if (RS232_OpenComport(cport_nr, bdrate, mode)) {
        //connection yoksa bi şey yap.
        qDebug("Can not open comport\n");

    }
    qDebug("Thread is working..");
    while (1) {
        while (1) {
            n = RS232_PollComport(cport_nr, buf, 1);

            if (n > 0) {
                buf[n] = 0;
                str[i] = buf[0];
                ++i;

                if (strcmp((char *) buf, "-") == 0) {

                    str[i - 1] = 0;
                    i = 0;

                    xPanel = atoi(strtok(str, ","));
                    yPanel = atoi(strtok(NULL, ","));
                    xMotor = atoi(strtok(NULL, ","));
                    char *temp = strtok(NULL, ",");
                    yMotor = atoi(temp);
                    //connect(drawingTh,SIGNAL(draw2D()),this,SLOT(update2DCoordinates()));
                    qDebug("xPanel: %d,\t yPanel: %d,\t xMotor:%d,\t yMotor:%d", xPanel, yPanel, xMotor, yMotor);
                    break;
                }
            }
        }

        //arduinodan gelen veriler strde

        sprintf(str, "%4d.%4d", x, y);
        ++x;
        ++y;

        if (x == 1000) {
            x = 0;
            y = 0;
        }

        RS232_cputs(cport_nr, str);
    }
    mutex.unlock();
}

void MainWindow::realtimeDataSlotFirst() {
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key - lastPointKey > 0.5) // at most add point every 2 ms
    {
        // add data to lines:
        ui->customPlot->graph(0)->addData(key, xPanel);
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
    if (key - lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
                QString("%1 FPS, Total Data points: %2")
                        .arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
                        .arg(ui->customPlot->graph(0)->data()->size()/*ui->customPlot->graph(1)->data()->size()*/), 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}

void MainWindow::realtimeDataSlotSecond() {
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key - lastPointKey > 0.5) // at most add point every 2 ms
    {
        // add data to lines:
        ui->customPlotSecond->graph(0)->addData(key, yPanel);
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
    if (key - lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
                QString("%1 FPS, Total Data points: %2")
                        .arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
                        .arg(ui->customPlotSecond->graph(
                                0)->data()->size()/*ui->customPlot->graph(1)->data()->size()*/), 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}

