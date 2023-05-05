#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "msg.h"
#include "myserial.h"
#include "recodefile.h"
#include "startplot.h"
#include"imualgorithms.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void uiconfig();

    void data_process(uint8_t* _data);

private slots:
    void on_pushButton_open_clicked();

    void on_pushButton_send_clicked();



    void on_recodeON_clicked();

    void on_recodeOFF_clicked();

    void on_startplotON_clicked();

private:
    Ui::MainWindow *ui;
    recodeFile *myrecode;

    mySerial *myserial;
    QThread *t1;
    QThread *t3;

    IMUAlgorithms *myImuAlgorithms;

    startPlot * my_plot;



signals:
    void s_open_serial();
    void s_indexs(serial_set_indexs);


};
#endif // MAINWINDOW_H
