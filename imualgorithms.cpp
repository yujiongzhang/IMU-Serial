#include "imualgorithms.h"
#include <QDebug>


IMUAlgorithms::IMUAlgorithms(QObject *parent) : QObject(parent)
{
    initAlgorithms();//初始化陀螺仪偏差；
    set_dlta_time(50);
    CFfilter = new ComplementaryFilter;
}

void IMUAlgorithms::initAlgorithms()
{
    gyroX_bias = 0.14413;
    gyroY_bias = 0.5950027;
    gyroZ_bias = 0.07859045;

    accX_pre = 0;
    accY_pre = 0;
    accZ_pre = 0;
    gyroX_pre = 0;
    gyroY_pre = 0;
    gyroZ_pre = 0;

}

void IMUAlgorithms::set_dlta_time(int _imu_rate)
{
    dlta_time = (double)(1.0/_imu_rate);
    qDebug()<<"当前频率为："<<_imu_rate;
}

void IMUAlgorithms::CF(IMU_Msg _imu_msg)
{
    double q0=0,q1=0,q2=0,q3=0;
    double pitch=0,roll=0,yaw=0;

    IMU_Euler_Msg imu_euler_msg;

    imu_euler_msg.accX = _imu_msg.accX*9.8;
    imu_euler_msg.accY = _imu_msg.accY*9.8;
    imu_euler_msg.accZ = _imu_msg.accZ*9.8;

    imu_euler_msg.gyroX =((_imu_msg.gyroX - gyroX_bias)/180)*3.1415926;
    imu_euler_msg.gyroY =((_imu_msg.gyroY - gyroY_bias)/180)*3.1415926;
    imu_euler_msg.gyroZ =((_imu_msg.gyroZ - gyroZ_bias)/180)*3.1415926;

//-------------------------------------------
    if (_imu_msg.accX == 0)
    {
       imu_euler_msg.accX = accX_pre;
    }
    if (_imu_msg.accY == 0 && _imu_msg.accZ == 0 && _imu_msg.gyroX == 0 && _imu_msg.gyroY == 0 && _imu_msg.gyroZ == 0)
    {
        imu_euler_msg.accY = accY_pre;
        imu_euler_msg.accZ = accZ_pre;
        imu_euler_msg.gyroX = gyroX_pre;
        imu_euler_msg.gyroY = gyroY_pre;
        imu_euler_msg.gyroZ = gyroZ_pre;
     }
//--------------------------------------------

    imu_euler_msg.magX = _imu_msg.magX;
    imu_euler_msg.magY = _imu_msg.magY;
    imu_euler_msg.magZ = _imu_msg.magZ;

    imu_euler_msg.temprature = _imu_msg.temprature;


    CFfilter->update(imu_euler_msg.accX,  imu_euler_msg.accY,  imu_euler_msg.accZ,\
                     imu_euler_msg.gyroX, imu_euler_msg.gyroY, imu_euler_msg.gyroZ, dlta_time);

    CFfilter->getOrientation(q0,q1,q2,q3);

    quaternion2Euler(q0,q1,q2,q3,&pitch,&roll,&yaw);

//=---------------------------------------------
    accX_pre = imu_euler_msg.accX;
    accY_pre = imu_euler_msg.accY;
    accZ_pre = imu_euler_msg.accZ;
    gyroX_pre = imu_euler_msg.gyroX;
    gyroY_pre = imu_euler_msg.gyroY;
    gyroZ_pre = imu_euler_msg.gyroZ;
//-----------------------------------------------

    imu_euler_msg.pitch = (float)pitch;
    imu_euler_msg.roll  = (float)roll;
    imu_euler_msg.yaw   = (float)yaw;

//    qDebug()<<"pitch:"<<imu_euler_msg.pitch;
//    qDebug()<<"roll:"<<imu_euler_msg.roll;
//    qDebug()<<"yaw:"<<imu_euler_msg.yaw;

    emit(s_imu_euler_msg(imu_euler_msg));

}


