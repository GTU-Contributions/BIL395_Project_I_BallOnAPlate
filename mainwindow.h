#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void realtimeDataSlotFirst();
    void realtimeDataSlotSecond();

private:
  Ui::MainWindow *ui;
  QTimer dataTimerFirst;
  QTimer dataTimerSecond;
};

#endif // MAINWINDOW_H
