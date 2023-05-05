#include "myserial.h"
#include <QDebug>
#include<QThread>
#include "crc8_crc16.h"


mySerial::mySerial(QObject *parent) : QObject(parent)
{

}

mySerial::mySerial(QString port)
{
    ser = new QSerialPort(port);
    ser->setReadBufferSize(1024);

    ser_read_sate = 0;
    remain_data = 37;

    memset(&myImuMsg,0,sizeof(myImuMsg));
}

mySerial::~mySerial()
{
    qDebug()<<"mySerial已经释放";
}

void mySerial::setSerial(serial_set_indexs indexs)
{
    qDebug() << "子线程地址: " << QThread::currentThread();

    //设置波特率
    switch (indexs.baudrate) {
        case 0:
            this->ser->setBaudRate(QSerialPort::Baud115200);
            break;
        case 1:
            this->ser->setBaudRate(QSerialPort::Baud9600);
            break;
        case 2:
            this->ser->setBaudRate(QSerialPort::Baud19200);
            break;
        case 3:
            this->ser->setBaudRate(921600);
            break;
        case 4:
            this->ser->setBaudRate(230400);
            break;
        case 5:
            this->ser->setBaudRate(460800);
            break;
    }

    //设置数据位数
    switch (indexs.databits) {
        case 0:
            this->ser->setDataBits(QSerialPort::Data8);
            break;
        case 1:
            this->ser->setDataBits(QSerialPort::Data7);
            break;
        case 2:
            this->ser->setDataBits(QSerialPort::Data6);
            break;
    }

    //设置校验方式
    switch (indexs.parity) {
        case 0:
            this->ser->setParity(QSerialPort::NoParity);
            break;
        case 1:
            this->ser->setParity(QSerialPort::OddParity);
            break;
        case 2:
            this->ser->setParity(QSerialPort::EvenParity);
            break;
    }

    //设置停止位位数
    switch (indexs.stopbits) {
        case 0:
            this->ser->setStopBits(QSerialPort::OneStop);
            break;
        case 1:
            this->ser->setStopBits(QSerialPort::OneAndHalfStop);
            break;
        case 2:
            this->ser->setStopBits(QSerialPort::TwoStop);
            break;
    }
    //关闭流控
    this->ser->setFlowControl(QSerialPort::NoFlowControl);

    //连接处理接收到的数据的信号和槽
    connect(ser,&QSerialPort::readyRead,this,&mySerial::recv_data_process);

}


void mySerial::recv_data_process()
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
//                    qDebug()<<"选项1";
                    ser_read_sate = 1;
//                    qDebug()<<"ser size:"<<ser->size();
                    int readed_num = ser->size();
                    QByteArray buf = ser->read(readed_num);//读buff中剩余的内容 = ser->read(1);
                    memcpy(data+3,buf.data(),readed_num);
                    remain_data = remain_data - readed_num;//还有这么多要读
//                    qDebug()<<"remain_data"<<remain_data;
//                    qDebug()<<"选项1 over";
                    return;
                }
                else
                {
//                    qDebug()<<"选项2";
                    QByteArray buf = ser->read(34);
                    memcpy(data+3,buf.data(),34);

                    ser_read_sate = 0;
                    remain_data = 37;

                    uint16_t CRC16_verify = get_CRC16_check_sum(data,35);
                    uint16_t CRC16_receive = (uint16_t)((uint16_t)(data[35]<<8) |  (uint16_t)data[36]);

//                    qDebug()<<"CRC16_receive:"<<CRC16_receive;
//                    qDebug()<<"CRC16_verify:"<<CRC16_verify;


                    if(CRC16_receive == CRC16_verify)
                    {
//                        qDebug()<<"顺利接收数据";
                        this->data_process(data);
                    }

//                    for(int a= 0; a<40;a++)
//                    {
//                        qDebug()<<"16进制大写输出"<<QString("%1").arg(data[a],2,16,QLatin1Char('0')).toUpper();//不足8位补0
//                    }

                    memset(data,0,sizeof(data));
                }
            }

            else {
//                qDebug()<<"function "<<data[1]<<"   datalenth:"<<data[2];
            }
        }
    }
    else if(ser_read_sate == 1)//读取剩余的内容
    {
        while(ser->bytesAvailable())
        {
//            qDebug()<<"ser size:"<<ser->size();
//            qDebug()<<"remain_data"<<remain_data;
            if(ser->size()>=remain_data)
            {
//                qDebug()<<"选项3";
                QByteArray buf = ser->read(remain_data);//读buff中剩余的内容;
                memcpy(data+37-remain_data,buf.data(),remain_data);

                ser_read_sate = 0;
                remain_data = 37;

                uint16_t CRC16_verify = get_CRC16_check_sum(data,35);
                uint16_t CRC16_receive = (uint16_t)((uint16_t)(data[35]<<8) |  (uint16_t)data[36]);

//                qDebug()<<"CRC16_receive:"<<CRC16_receive;
//                qDebug()<<"CRC16_verify:"<<CRC16_verify;

//                for(int a= 0; a<40;a++)
//                {
//                    qDebug()<<"16进制大写输出"<<QString("%1").arg(data[a],2,16,QLatin1Char('0')).toUpper();//不足8位补0
//                }

                if(CRC16_receive == CRC16_verify)
                {
//                    qDebug()<<"顺利接收数据";
                    this->data_process(data);
                }
                memset(data,0,sizeof(data));

            }
            else{
//                qDebug()<<"选项4";
                int readed_num = ser->size();
                QByteArray buf = ser->read(readed_num);
                memcpy(data+37-remain_data,buf.data(),readed_num);
                remain_data = remain_data - readed_num;//还有这么多要读
            }
         }
    }
}

void mySerial::data_process(uint8_t *_data)
{
    myImuMsg.accX = ((float)(_data[3]<<24  | _data[4]<<16  | _data[5]<<8  | _data[6])  /26756268/9.8);
    myImuMsg.accY = ((float)(_data[7]<<24  | _data[8]<<16  | _data[9]<<8  | _data[10])/26756268/9.8);
    myImuMsg.accZ = ((float)(_data[11]<<24 | _data[12]<<16 | _data[13]<<8 | _data[14])/26756268/9.8);

    myImuMsg.gyroX = ((float)(_data[15]<<24 | _data[16]<<16 | _data[17]<<8 | _data[18])  /2621440);
    myImuMsg.gyroY = ((float)(_data[19]<<24 | _data[20]<<16 | _data[21]<<8 | _data[22])  /2621440);
    myImuMsg.gyroZ = ((float)(_data[23]<<24 | _data[24]<<16 | _data[25]<<8 | _data[26])  /2621440);

    myImuMsg.magX = (float)((int16_t)(_data[27]<<8 | _data[28])) /75;
    myImuMsg.magY = (float)((int16_t)(_data[29]<<8 | _data[30])) /75;
    myImuMsg.magZ = (float)((int16_t)(_data[31]<<8 | _data[32])) /75;

    myImuMsg.temprature = (float)((int16_t)(_data[33]<<8 | _data[34])) / 100;

    emit s_IMU_msg(myImuMsg);

//    qDebug() << "子线程地址: " << QThread::currentThread();
//    qDebug()<<"accX:"<<myImuMsg.accX;
//    qDebug()<<"accY:"<<myImuMsg.accY;
//    qDebug()<<"accZ:"<<myImuMsg.accZ;
//    qDebug()<<"gyroX:"<<myImuMsg.gyroX;
//    qDebug()<<"gyroy:"<<myImuMsg.gyroY;
//    qDebug()<<"gyroZ:"<<myImuMsg.gyroZ;
//    qDebug()<<"magX:"<<myImuMsg.magX;
//    qDebug()<<"magY:"<<myImuMsg.magY;
//    qDebug()<<"magZ:"<<myImuMsg.magZ;
//    qDebug()<<"temprature:"<<myImuMsg.temprature;

}

void mySerial::closeSerial()
{

    this->ser->clear();
    this->ser->close();
    this->ser->deleteLater();
    qDebug()<<"ser已经释放";
}

