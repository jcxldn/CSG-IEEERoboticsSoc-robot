#ifndef SRC_MPU_CONTROLLER_H_
#define SRC_MPU_CONTROLLER_H_

// use fastwire (saves ~2k flash)
// uncomment to use arduino wire library
#define I2CDEV_IMPLEMENTATION I2CDEV_BUILTIN_FASTWIRE
// #define I2CDEV_SERIAL_DEBUG 1

#include <MPU6050_6Axis_MotionApps612.h>

#include "pixel.h"

enum MPUState
{
    READY,
    CONNECTED,
    ERROR_INIT,
    ERROR_CONFIGURE
};

class MPUMeasurements
{
public:
    uint8_t FIFOBuffer[64];
    Quaternion q;   // [w, x, y, z]         Quaternion container
    VectorInt16 aa; // [x, y, z]            Accel sensor measurements
    // VectorInt16 gy;      // [x, y, z]            Gyro sensor measurements
    VectorInt16 aaReal;  // [x, y, z]            Gravity-free accel sensor measurements
    VectorInt16 aaWorld; // [x, y, z]            World-frame accel sensor measurements
    VectorFloat gravity; // [x, y, z]            Gravity vector
    float ypr[3];        // [yaw, pitch, roll]   Yaw/Pitch/Roll container and gravity vector

    float getDegrees();
};

class MPUController
{
private:
    MPUState initMpu();
    MPUState configureMpu();

    Pixel *p;

    MPUState state;

    MPU6050_6Axis_MotionApps612 mpu;
    uint8_t device_state;
    uint16_t packet_size;
    uint16_t fifo_count;

public:
    MPUController(Pixel *p);
    boolean read();

    MPUMeasurements measurements;
};

#endif /* SRC_MPU_CONTROLLER_H_ */