#include "startplot.h"
#include "ui_startplot.h"

startPlot::startPlot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::startPlot)
{
    ui->setupUi(this);

    setupRealtimeDataDemo1(ui->customPlot);//画图窗口1设置
    setupRealtimeDataDemo2(ui->customPlot2);//画图窗口2设置
    setupRealtimeDataDemo3(ui->customPlot3);//画图窗口2设置

    qDebug() << "startplot线程地址: " << QThread::currentThread();
    qDebug() << "创建 startPlot 类" ;

    // 设置一个重复调用 MainWindow::realtimeDataSlot 的计时器
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(timeDataSlot()));
    dataTimer.start(0); // Interval 0 means to refresh as fast as possible

    this->setAttribute(Qt::WA_DeleteOnClose,true);
}

startPlot::~startPlot()
{
    delete ui;
    qDebug() << "startPlot 类被释放" ;
}


void startPlot::setupRealtimeDataDemo1(QCustomPlot *customPlot)
{
    customPlot->addGraph(); // 添加图形1
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));//设置颜色
    customPlot->addGraph(); // 添加图形2
    customPlot->graph(1)->setPen(QPen(QColor(255, 40, 110)));//设置颜色
    customPlot->addGraph(); // 添加图形3
    customPlot->graph(2)->setPen(QPen(QColor(110, 255, 40)));//设置颜色

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime); //设置一个 QCPAxisTickerTime 类的共享指针
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-10, 20);

    // 使左轴和下轴将其范围转移到右轴和上轴
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
}


void startPlot::setupRealtimeDataDemo2(QCustomPlot *customPlot)
{
    customPlot->addGraph(); // 添加图形1
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));//设置颜色
    customPlot->addGraph(); // 添加图形2
    customPlot->graph(1)->setPen(QPen(QColor(255, 40, 110)));//设置颜色
    customPlot->addGraph(); // 添加图形3
    customPlot->graph(2)->setPen(QPen(QColor(110, 255, 40)));//设置颜色

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime); //设置一个 QCPAxisTickerTime 类的共享指针
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-30,30);

    // 使左轴和下轴将其范围转移到右轴和上轴
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
}


void startPlot::setupRealtimeDataDemo3(QCustomPlot *customPlot)
{
    customPlot->addGraph(); // 添加图形1
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));//设置颜色
    customPlot->addGraph(); // 添加图形2
    customPlot->graph(1)->setPen(QPen(QColor(255, 40, 110)));//设置颜色
    customPlot->addGraph(); // 添加图形3
    customPlot->graph(2)->setPen(QPen(QColor(110, 255, 40)));//设置颜色

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime); //设置一个 QCPAxisTickerTime 类的共享指针
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-180, 180);

    // 使左轴和下轴将其范围转移到右轴和上轴
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

void startPlot::addDataSlot1(double _pitch, double _roll, double _yaw)
{
    ui->customPlot->graph(0)->addData(this->t, _pitch);
    ui->customPlot->graph(1)->addData(this->t, _roll);
    ui->customPlot->graph(2)->addData(this->t, _yaw);

//    ui->customPlot->graph(0)->rescaleValueAxis();//重新缩放值（垂直）轴以适合当前数据
//    ui->customPlot->graph(1)->rescaleValueAxis();//重新缩放值（垂直）轴以适合当前数据
//    ui->customPlot->graph(2)->rescaleValueAxis();//重新缩放值（垂直）轴以适合当前数据

    ui->customPlot->xAxis->setRange(this->t, 10, Qt::AlignRight);// 使键轴范围随数据滚动（恒定范围大小为8）：
    ui->customPlot->replot();
}



void startPlot::addDataSlot2(float num1, float num2, float num3)
{
    ui->customPlot2->graph(0)->addData(this->t, num1);
    ui->customPlot2->graph(1)->addData(this->t, num2);
    ui->customPlot2->graph(2)->addData(this->t, num3);

    ui->customPlot2->xAxis->setRange(this->t, 10, Qt::AlignRight);// 使键轴范围随数据滚动（恒定范围大小为8）：
    ui->customPlot2->replot();
}

void startPlot::addDataSlot3(float num1, float num2, float num3)
{
    ui->customPlot3->graph(0)->addData(this->t, num1);
    ui->customPlot3->graph(1)->addData(this->t, num2);
    ui->customPlot3->graph(2)->addData(this->t, num3);

    ui->customPlot3->xAxis->setRange(this->t, 10, Qt::AlignRight);// 使键轴范围随数据滚动（恒定范围大小为8）：
    ui->customPlot3->replot();
}

void startPlot::updateSlot(IMU_Euler_Msg _imu_euler_msg)
{
    addDataSlot1(_imu_euler_msg.accX,_imu_euler_msg.accY,_imu_euler_msg.accZ);
    addDataSlot2(_imu_euler_msg.gyroX,_imu_euler_msg.gyroY,_imu_euler_msg.gyroZ);
    addDataSlot3(_imu_euler_msg.pitch,_imu_euler_msg.roll,_imu_euler_msg.yaw);
}


//通过时间来创建一个虚拟的模拟下位机上传
void startPlot::timeDataSlot()
{
    static QTime timeStart2 = QTime::currentTime();
    t = timeStart2.msecsTo(QTime::currentTime())/1000.0; // 项目开始后经过的时间,单位 s
}
