#ifndef RECODEFILE_H
#define RECODEFILE_H

#include <QObject>
#include <QtDebug>
#include "msg.h"
#include<QTime>

class recodeFile : public QObject
{
    Q_OBJECT
public:
    explicit recodeFile(QObject *parent = nullptr);
    ~recodeFile();

    void writeRecode(IMU_Euler_Msg _imu_euler_msg);


signals:

private:
    FILE* fp;

};

#endif // RECODEFILE_H
