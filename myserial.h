#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>         // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>     // 提供系统中存在的串口信息
#include <msg.h>

struct serial_set_indexs
{
    qint32 baudrate;
    qint32 databits;
    qint32 parity;
    qint32 stopbits;
};

class mySerial : public QObject
{
    Q_OBJECT
public:
    explicit mySerial(QObject *parent = nullptr);
    mySerial(QString port);
    ~mySerial();

    void setSerial(serial_set_indexs indexs);

    void recv_data_process();

    void data_process(uint8_t *_data);

    void closeSerial();


    QSerialPort *ser;



signals:
    void s_IMU_msg(IMU_Msg);


private:

    struct IMU_Msg myImuMsg;

    int ser_read_sate;//0表示从新开始读，1表示上一次有没读完的
    int remain_data;//剩余多少个字节没读
    uint8_t data[40];

};

#endif // MYSERIAL_H
