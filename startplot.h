#ifndef STARTPLOT_H
#define STARTPLOT_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "msg.h"

namespace Ui {
class startPlot;
}

class startPlot : public QMainWindow
{
    Q_OBJECT

public:
    explicit startPlot(QWidget *parent = nullptr);
    ~startPlot();
    void setupRealtimeDataDemo1(QCustomPlot *customPlot);//画图窗口1设置
    void addDataSlot1(double _pitch, double _roll, double _yaw);                       //添加画图窗口1中数据
    void setupRealtimeDataDemo2(QCustomPlot *customPlot);//画图窗口2设置
    void addDataSlot2(double mun);                       //添加画图窗口2中数据
    void addDataSlot2(float num1, float num2, float num3);

    void setupRealtimeDataDemo3(QCustomPlot *customPlot);//画图窗口2设置
    void addDataSlot3(float num1, float num2, float num3);                       //添加画图窗口2中数据

    void updateSlot(IMU_Euler_Msg _imu_euler_msg);//更新窗口


private:
    Ui::startPlot *ui;
    QTimer dataTimer;

    double t;

private slots:
    void timeDataSlot();

};

#endif // STARTPLOT_H
