#include "recodefile.h"
#include<QThread>

recodeFile::recodeFile(QObject *parent) : QObject(parent)
{
    fp = fopen("recode.txt","w");
    if(fp == NULL){
        qDebug()<<"file error";
    }
}

recodeFile::~recodeFile()
{
    fclose(fp);
    qDebug()<<"file close successful!";
}

void recodeFile::writeRecode(IMU_Euler_Msg _imu_euler_msg)
{
    qDebug() << "数据记录子线程地址: " << QThread::currentThread();

    QString imu_euler_msg_to_str;
    imu_euler_msg_to_str.append(QTime::currentTime().toString("HH:mm:ss:zzz"));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.accX,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.accY,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.accZ,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.gyroX,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.gyroY,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.gyroZ,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.magX,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.magY,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.magZ,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.pitch,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.roll,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.yaw,'f'));
    imu_euler_msg_to_str.append("  ");
    imu_euler_msg_to_str.append( QString::number(_imu_euler_msg.temprature,'f'));
    imu_euler_msg_to_str.append("\n");

    QByteArray imu_msg_to_chr = imu_euler_msg_to_str.toLatin1();

    fputs(imu_msg_to_chr.data(),fp);

}

