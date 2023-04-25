#ifndef MSG_H
#define MSG_H
#pragma pack(1)     //设置结构体为1字节对齐

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;

/*Unsigned*/
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;




//struct IMU_Msg{
//    int8_t header;
//    int8_t type;
//    int8_t data1l;
//    int8_t data1h;
//    int8_t data2l;
//    int8_t data2h;
//    int8_t data3l;
//    int8_t data3h;
//    int8_t data4l;
//    int8_t data4h;
//    int8_t sumcrc;
//};


struct IMU_Msg_uint8{
    uint8_t ID;
    uint8_t function;
    uint8_t dlen;

    uint8_t accXH1;
    uint8_t accXH2;
    uint8_t accXL1;
    uint8_t accXL2;
    uint8_t accYH1;
    uint8_t accYH2;
    uint8_t accYL1;
    uint8_t accYL2;
    uint8_t accZH1;
    uint8_t accZH2;
    uint8_t accZL1;
    uint8_t accZL2;

    uint8_t gyroXH1;
    uint8_t gyroXH2;
    uint8_t gyroXL1;
    uint8_t gyroXL2;
    uint8_t gyroYH1;
    uint8_t gyroYH2;
    uint8_t gyroYL1;
    uint8_t gyroYL2;
    uint8_t gyroZH1;
    uint8_t gyroZH2;
    uint8_t gyroZL1;
    uint8_t gyroZL2;

    uint8_t magXH;
    uint8_t magXL;
    uint8_t magYH;
    uint8_t magYL;
    uint8_t magZH;
    uint8_t magZL;

    uint8_t tempH;
    uint8_t tempL;

    uint8_t CRC16H;
    uint8_t CRC16L;

};


struct IMU_Msg{
    uint8_t ID;
    uint8_t function;
    uint8_t dlen;

    float accX;
    float accY;
    float accZ;

    float gyroX;
    float gyroY;
    float gyroZ;

    float magX;
    float magY;
    float magZ;

    float temprature;

    uint16_t CRC16;

};

struct IMU_Euler_Msg{

    float accX;
    float accY;
    float accZ;

    float gyroX;
    float gyroY;
    float gyroZ;

    float magX;
    float magY;
    float magZ;

    float pitch;
    float roll;
    float yaw;

    float temprature;

};


#pragma pack()      //结束结构体对齐设置
#endif // MSG_H
