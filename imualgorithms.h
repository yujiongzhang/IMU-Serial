#ifndef IMUALGORITHMS_H
#define IMUALGORITHMS_H

#include <QObject>
#include "complementary_filter.h"
#include "msg.h"
#include<string.h>
using namespace imu_tools;


class IMUAlgorithms : public QObject
{
    Q_OBJECT
public:
    explicit IMUAlgorithms(QObject *parent = nullptr);
    void initAlgorithms();
    void set_dlta_time(int _imu_rate);

    void CF(IMU_Msg _imu_msg);

signals:
    void s_imu_euler_msg(IMU_Euler_Msg);

private:
    float gyroX_bias;
    float gyroY_bias;
    float gyroZ_bias;

    float dlta_time;

};

#endif // IMUALGORITHMS_H
