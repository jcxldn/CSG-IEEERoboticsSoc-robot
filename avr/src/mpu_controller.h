#ifndef SRC_MPU_CONTROLLER_H_
#define SRC_MPU_CONTROLLER_H_

// use fastwire (saves ~2k flash)
// uncomment to use arduino wire library
#define I2CDEV_IMPLEMENTATION I2CDEV_BUILTIN_FASTWIRE

#include <MPU6050.h>

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
    MPU6050 mpu;

public:
    MPUController(Pixel *p);
};

#endif /* SRC_MPU_CONTROLLER_H_ */