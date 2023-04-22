#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>         // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>     // 提供系统中存在的串口信息
#include <QDebug>
#include "msg.h"

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
    void recv_data_process();
    void data_process(uint8_t* _data);

private slots:
    void on_pushButton_open_clicked();

    void on_pushButton_send_clicked();



private:
    Ui::MainWindow *ui;
    QSerialPort *ser;

    int ser_read_sate;//0表示从新开始读，1表示上一次有没读完的
    int remain_data;//剩余多少个字节没读
    uint8_t data[40];

};
#endif // MAINWINDOW_H
