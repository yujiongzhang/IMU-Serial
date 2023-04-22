#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "crc8_crc16.h"
#include <QThread>

#include <QMessageBox>
#include <QThread>


struct IMU_Msg myImuMsg;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    uiconfig();

    qDebug() << "主线程地址: " << QThread::currentThread();

    memset(&myImuMsg,0,sizeof(myImuMsg));


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
    if(ui->pushButton_open->text()=="打开串口"){
        ser = new QSerialPort(ui->comboBox_port->currentText());
        ui->pushButton_open->setText("关闭串口");
        //打开串口
        if(!ser->open(QIODevice::ReadWrite)){
            QMessageBox::warning(this,"提示","打开串口失败！");
            ser->deleteLater();
            return;
        }
        //设置波特率
        switch (ui->comboBox_baudrate->currentIndex()) {
            case 0:
                ser->setBaudRate(QSerialPort::Baud115200);
                break;
            case 1:
                ser->setBaudRate(QSerialPort::Baud9600);
                break;
            case 2:
                ser->setBaudRate(QSerialPort::Baud19200);
                break;
        }

        //设置数据位数
        switch (ui->comboBox_databits->currentIndex()) {
            case 0:
                ser->setDataBits(QSerialPort::Data8);
                break;
            case 1:
                ser->setDataBits(QSerialPort::Data7);
                break;
            case 2:
                ser->setDataBits(QSerialPort::Data6);
                break;
        }

        //设置校验方式
        switch (ui->comboBox_parity->currentIndex()) {
            case 0:
                ser->setParity(QSerialPort::NoParity);
                break;
            case 1:
                ser->setParity(QSerialPort::OddParity);
                break;
            case 2:
                ser->setParity(QSerialPort::EvenParity);
                break;
        }

        //设置停止位位数
        switch (ui->comboBox_stopbits->currentIndex()) {
            case 0:
                ser->setStopBits(QSerialPort::OneStop);
                break;
            case 1:
                ser->setStopBits(QSerialPort::OneAndHalfStop);
                break;
            case 2:
                ser->setStopBits(QSerialPort::TwoStop);
                break;
        }

        //关闭流控
        ser->setFlowControl(QSerialPort::NoFlowControl);

        //禁止和使能控件
        ui->comboBox_port->setEnabled(false);
        ui->comboBox_baudrate->setEnabled(false);
        ui->comboBox_databits->setEnabled(false);
        ui->comboBox_parity->setEnabled(false);
        ui->comboBox_stopbits->setEnabled(false);
        ui->pushButton_send->setEnabled(true);

        //连接处理接收到的数据的信号和槽
        ser_read_sate = 0;
        remain_data = 37;
        connect(ser,&QSerialPort::readyRead,this,&MainWindow::recv_data_process);

    }
    else{//关闭串口
        ser->clear();
        ser->close();
        ser->deleteLater();

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
}


void MainWindow::recv_data_process()
{


//-------------------------------------------------
//-----------打印全部的接收信息-----------------------
//    QByteArray arr = ser->readAll();
//    qDebug()<<"size : "<<arr.size();

//    memcpy(data,arr.data(),arr.size());

//    for(int i=0; i<arr.size();i++)
//    {
//        qDebug()<<data[i];
//    }
//-----------打印全部的接收信息------------------------
//--------------------------------------------------



    if(ser_read_sate == 0)//从头开始读
    {
        while(ser->bytesAvailable())
        {
            QByteArray header = ser->read(1);
            memcpy(data,header.data(),1);
            while (data[0] != 0x50)
            {
                qDebug()<<"header error";
                if(ser->bytesAvailable())
                {
                    QByteArray header = ser->read(1);
                    memcpy(data,header.data(),1);
                }
                else{
                    return;
                }
            }

            QByteArray function = ser->read(1);
            memcpy(data+1,function.data(),1);
            QByteArray dlen = ser->read(1);
            memcpy(data+2,dlen.data(),1);
            remain_data = remain_data - 3;

            if(data[1]==0x03 && data[2]==32)
            {

                if(ser->size() < remain_data )//出现分包的问题
                {
                    qDebug()<<"选项1";
                    ser_read_sate = 1;
                    qDebug()<<"ser size:"<<ser->size();
                    int readed_num = ser->size();
                    QByteArray buf = ser->read(readed_num);//读buff中剩余的内容 = ser->read(1);
                    memcpy(data+3,buf.data(),readed_num);
                    remain_data = remain_data - readed_num;//还有这么多要读
                    qDebug()<<"remain_data"<<remain_data;
                    qDebug()<<"选项1 over";
                    return;
                }
                else
                {
                    qDebug()<<"选项2";
                    QByteArray buf = ser->read(34);
                    memcpy(data+3,buf.data(),34);

                    ser_read_sate = 0;
                    remain_data = 37;

                    uint16_t CRC16_verify = get_CRC16_check_sum(data,35);
                    uint16_t CRC16_receive = (uint16_t)((uint16_t)(data[35]<<8) |  (uint16_t)data[36]);

                    qDebug()<<"CRC16_receive:"<<CRC16_receive;
                    qDebug()<<"CRC16_verify:"<<CRC16_verify;



                    if(CRC16_receive == CRC16_verify)
                    {
                        qDebug()<<"顺利接收数据";
                        this->data_process(data);
                    }

                    for(int a= 0; a<40;a++)
                    {
                        qDebug()<<"16进制大写输出"<<QString("%1").arg(data[a],2,16,QLatin1Char('0')).toUpper();//不足8位补0
                    }

                    memset(data,0,sizeof(data));
                }
            }

            else {
                qDebug()<<"function "<<data[1]<<"   datalenth:"<<data[2];
            }
        }
    }
    else if(ser_read_sate == 1)//读取剩余的内容
    {
        while(ser->bytesAvailable())
        {
            qDebug()<<"ser size:"<<ser->size();
            qDebug()<<"remain_data"<<remain_data;
            if(ser->size()>=remain_data)
            {
                qDebug()<<"选项3";
                QByteArray buf = ser->read(remain_data);//读buff中剩余的内容;
                memcpy(data+37-remain_data,buf.data(),remain_data);

                ser_read_sate = 0;
                remain_data = 37;

                uint16_t CRC16_verify = get_CRC16_check_sum(data,35);
//                uint16_t CRC16_receive;
//                memcpy(&CRC16_receive,data+35,2);
                uint16_t CRC16_receive = (uint16_t)((uint16_t)(data[35]<<8) |  (uint16_t)data[36]);

                qDebug()<<"CRC16_receive:"<<CRC16_receive;
                qDebug()<<"CRC16_verify:"<<CRC16_verify;

                for(int a= 0; a<40;a++)
                {
                    qDebug()<<"16进制大写输出"<<QString("%1").arg(data[a],2,16,QLatin1Char('0')).toUpper();//不足8位补0
                }

                if(CRC16_receive == CRC16_verify)
                {
                    qDebug()<<"顺利接收数据";
                    this->data_process(data);
                }
                memset(data,0,sizeof(data));

            }
            else{
                qDebug()<<"选项4";
                int readed_num = ser->size();
                QByteArray buf = ser->read(readed_num);
                memcpy(data+37-remain_data,buf.data(),readed_num);
                remain_data = remain_data - readed_num;//还有这么多要读
            }
         }
    }
}

void MainWindow::data_process(uint8_t *_data)
{
    myImuMsg.accX = ((float)(_data[3]<<24  | _data[4]<<16  | _data[5]<<8  | _data[6])  /26756268/9.8);
    myImuMsg.accY = ((float)(_data[7]<<24  | _data[8]<<16  | _data[9]<<8  | _data[10])/26756268/9.8);
    myImuMsg.accZ = ((float)(_data[11]<<24 | _data[12]<<16 | _data[13]<<8 | _data[14])/26756268/9.8);

    myImuMsg.gyroX = ((float)(_data[15]<<24 | _data[16]<<16 | _data[17]<<8 | _data[18])  /2621440);
    myImuMsg.gyroY = ((float)(_data[19]<<24 | _data[20]<<16 | _data[21]<<8 | _data[22])  /2621440);
    myImuMsg.gyroZ = ((float)(_data[23]<<24 | _data[24]<<16 | _data[25]<<8 | _data[26])  /2621440);

    myImuMsg.magX = (float)(_data[27]<<8 | _data[28]) /75;
    myImuMsg.magY = (float)(_data[29]<<8 | _data[30]) /75;
    myImuMsg.magZ = (float)(_data[31]<<8 | _data[31]) /75;

    myImuMsg.temprature = (float)(_data[32]<<8 | _data[33]) / 100;

    qDebug()<<"accX:"<<myImuMsg.accX;
    qDebug()<<"accY:"<<myImuMsg.accY;
    qDebug()<<"accZ:"<<myImuMsg.accZ;

    qDebug()<<"gyroX:"<<myImuMsg.gyroX;
    qDebug()<<"gyroy:"<<myImuMsg.gyroY;
    qDebug()<<"gyroZ:"<<myImuMsg.gyroZ;

    qDebug()<<"magX:"<<myImuMsg.magX;
    qDebug()<<"magY:"<<myImuMsg.magY;
    qDebug()<<"magZ:"<<myImuMsg.magZ;

    qDebug()<<"temprature:"<<myImuMsg.temprature;
}





void MainWindow::on_pushButton_send_clicked()
{
    //获取要发送的数据
    QByteArray arr = ui->textEdit->toPlainText().toUtf8();
    ser->write(arr);
}





