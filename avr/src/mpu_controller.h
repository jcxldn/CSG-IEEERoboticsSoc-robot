#ifndef SRC_MPU_CONTROLLER_H_
#define SRC_MPU_CONTROLLER_H_

// use fastwire (saves ~2k flash)
// uncomment to use arduino wire library
#define I2CDEV_IMPLEMENTATION I2CDEV_BUILTIN_FASTWIRE
// #define I2CDEV_SERIAL_DEBUG 1

#include <MPU6050_6Axis_MotionApps20.h>

#include "pixel.h"

enum MPUState
{
    CONNECTED,
    ERROR
};

class MPUController
{
private:
    MPUState init();

        Pixel *p;

public:
    MPUController(Pixel *p);
    static void task(void *pvParameters);

    // public so task context can access
    MPU6050_6Axis_MotionApps20 mpu;
    uint8_t device_state;
    uint16_t packet_size;
    uint16_t fifo_count;
};

#endif /* SRC_MPU_CONTROLLER_H_ */