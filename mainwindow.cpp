#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>
#include <QThread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<serial_set_indexs>("serial_set_indexs");//跨线程的信号和槽的参数传递中,参数的类型是自定义的类型
    qRegisterMetaType<IMU_Msg>("IMU_Msg");//跨线程的信号和槽的参数传递中,参数的类型是自定义的类型

    uiconfig();

    qDebug() << "主线程地址: " << QThread::currentThread();


    //获取有效的串口
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        qDebug()<<info.portName()<<info.description();


        //串口是否空闲
        QSerialPort serial(info);
        if(serial.open(QIODevice::ReadWrite)){
            ui->comboBox_port->addItem(info.portName());
            serial.close();//关闭串口
        }
        else{
            ui->comboBox_port->addItem(info.portName()+"(被占用)");
        }
    }


    connect(this,&MainWindow::s_open_serial,this,[=]()
    {
        if(ui->pushButton_open->text()=="打开串口"){

            t1 = new QThread;
            myserial = new mySerial(ui->comboBox_port->currentText());
            myserial -> moveToThread(t1);
            t1->start();

            connect(this,&MainWindow::s_indexs,myserial,&mySerial::setSerial);

            //打开串口
            if(!myserial->ser->open(QIODevice::ReadWrite)){
                QMessageBox::warning(this,"提示","打开串口失败！");
                return;
            }

            serial_set_indexs my_serial_set_indexs = {0,0,0,0};
            my_serial_set_indexs.baudrate = ui->comboBox_baudrate->currentIndex();
            my_serial_set_indexs.databits = ui->comboBox_databits->currentIndex();
            my_serial_set_indexs.parity = ui->comboBox_parity->currentIndex();
            my_serial_set_indexs.stopbits = ui->comboBox_stopbits->currentIndex();

            emit(s_indexs(my_serial_set_indexs));

            //禁止和使能控件
            ui->comboBox_port->setEnabled(false);
            ui->comboBox_baudrate->setEnabled(false);
            ui->comboBox_databits->setEnabled(false);
            ui->comboBox_parity->setEnabled(false);
            ui->comboBox_stopbits->setEnabled(false);
            ui->pushButton_send->setEnabled(true);

            //按键显示修改
            ui->pushButton_open->setText("关闭串口");

        }
        else{//关闭串口


            myserial->closeSerial();
            t1->quit();
            t1->wait();
            myserial->deleteLater();
            t1->deleteLater();


            //禁止和使能控件
            ui->comboBox_port->setEnabled(true);
            ui->comboBox_baudrate->setEnabled(true);
            ui->comboBox_databits->setEnabled(true);
            ui->comboBox_parity->setEnabled(true);
            ui->comboBox_stopbits->setEnabled(true);
            ui->pushButton_send->setEnabled(false);

            //按键显示修改
            ui->pushButton_open->setText("打开串口");
        }
    });



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::uiconfig()
{
    ui->comboBox_baudrate->addItem("115200");
    ui->comboBox_baudrate->addItem("9600");
    ui->comboBox_baudrate->addItem("19200");

    ui->comboBox_databits->addItem("8");
    ui->comboBox_databits->addItem("7");
    ui->comboBox_databits->addItem("6");

    ui->comboBox_parity->addItem("None");
    ui->comboBox_parity->addItem("odd");
    ui->comboBox_parity->addItem("eve");

    ui->comboBox_stopbits->addItem("1");
    ui->comboBox_stopbits->addItem("1.5");
    ui->comboBox_stopbits->addItem("2");
}




void MainWindow::on_pushButton_open_clicked()
{
    emit(s_open_serial());
}



void MainWindow::on_pushButton_send_clicked()
{
    //获取要发送的数据
    QByteArray arr = ui->textEdit->toPlainText().toUtf8();
    myserial->ser->write(arr);
}






void MainWindow::on_recodeON_clicked()
{
    myrecode = new recodeFile;
    connect(myserial,&mySerial::s_IMU_msg,myrecode,&recodeFile::writeRecode);
}

void MainWindow::on_recodeOFF_clicked()
{
    myrecode->deleteLater();
}

