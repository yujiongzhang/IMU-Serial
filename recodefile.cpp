#include "recodefile.h"

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

void recodeFile::writeRecode(IMU_Msg _imu_msg)
{
    QString imu_msg_to_str;
    imu_msg_to_str.append(QTime::currentTime().toString("HH:mm:ss:zzz"));
    imu_msg_to_str.append("  ");
    imu_msg_to_str.append( QString::number(_imu_msg.accX,'f'));
    imu_msg_to_str.append("  ");
    imu_msg_to_str.append( QString::number(_imu_msg.accY,'f'));
    imu_msg_to_str.append("  ");
    imu_msg_to_str.append( QString::number(_imu_msg.accZ,'f'));
    imu_msg_to_str.append("  ");
    imu_msg_to_str.append( QString::number(_imu_msg.gyroX,'f'));
    imu_msg_to_str.append("  ");
    imu_msg_to_str.append( QString::number(_imu_msg.gyroY,'f'));
    imu_msg_to_str.append("  ");
    imu_msg_to_str.append( QString::number(_imu_msg.gyroZ,'f'));
    imu_msg_to_str.append("  ");
    imu_msg_to_str.append( QString::number(_imu_msg.magX,'f'));
    imu_msg_to_str.append("  ");
    imu_msg_to_str.append( QString::number(_imu_msg.magY,'f'));
    imu_msg_to_str.append("  ");
    imu_msg_to_str.append( QString::number(_imu_msg.magZ,'f'));
    imu_msg_to_str.append("  ");
    imu_msg_to_str.append( QString::number(_imu_msg.temprature,'f'));
    imu_msg_to_str.append("\n");

    QByteArray imu_msg_to_chr = imu_msg_to_str.toLatin1();
    fputs(imu_msg_to_chr.data(),fp);
}
