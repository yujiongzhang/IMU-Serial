#include "imualgorithms.h"
#include <QDebug>


IMUAlgorithms::IMUAlgorithms(QObject *parent) : QObject(parent)
{
    initAlgorithms();//初始化陀螺仪偏差；
}

void IMUAlgorithms::initAlgorithms()
{
    gyroX_bias = 0.14413;
    gyroY_bias = 0.5950027;
    gyroZ_bias = 0.07859045;


}

void IMUAlgorithms::set_dlta_time(int _imu_rate)
{
    _imu_rate = 100;
    dlta_time = 1.0/_imu_rate;
}

void IMUAlgorithms::CF(IMU_Msg _imu_msg)
{


    double q0,q1,q2,q3;
    double pitch=0,roll=0,yaw=0;
    IMU_Euler_Msg imu_euler_msg;

    imu_euler_msg.accX = _imu_msg.accX * 9.8;
    imu_euler_msg.accY = _imu_msg.accY * 9.8;
    imu_euler_msg.accZ = _imu_msg.accZ * 9.8;

    imu_euler_msg.gyroX = ((_imu_msg.gyroX - gyroX_bias)/180)*3.1415926;
    imu_euler_msg.gyroY = ((_imu_msg.gyroY - gyroY_bias)/180)*3.1415926;
    imu_euler_msg.gyroZ = ((_imu_msg.gyroZ - gyroZ_bias)/180)*3.1415926;

    imu_euler_msg.magX = _imu_msg.magX;
    imu_euler_msg.magY = _imu_msg.magY;
    imu_euler_msg.magZ = _imu_msg.magZ;

    imu_euler_msg.temprature = _imu_msg.temprature;


    CFfilter.update(imu_euler_msg.accX,  imu_euler_msg.accY,  imu_euler_msg.accZ,\
                    imu_euler_msg.gyroX, imu_euler_msg.gyroY, imu_euler_msg.gyroZ, dlta_time);

    CFfilter.getOrientation(q0,q1,q2,q3);

    quaternion2Euler(q0,q1,q2,q3,&pitch,&roll,&yaw);

    imu_euler_msg.pitch = (float)pitch;
    imu_euler_msg.roll  = (float)roll;
    imu_euler_msg.yaw   = (float)yaw;

    qDebug()<<"pitch:"<<imu_euler_msg.pitch;
    qDebug()<<"roll:"<<imu_euler_msg.roll;
    qDebug()<<"yaw:"<<imu_euler_msg.yaw;

    emit(s_imu_euler_msg(imu_euler_msg));

}
