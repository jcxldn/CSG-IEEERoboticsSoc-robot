// Arduino Core
#include <Arduino.h>

#include "mpu_controller.h"

// CRGB
#include <FastLED.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
#include <I2Cdev.cpp>
#endif

// https://github.com/embeddedartistry/arduino-printf - printf()
#include <LibPrintf.h>

MPUController::MPUController(Pixel *p)
{
    MPUState state = this->init();

    if (state == MPUState::CONNECTED)
    {
        // Continue

        printf("%s: Successfully connected to MPU.\n", __PRETTY_FUNCTION__);
    }
    else
    {
        // Show error
        printf("%s: Unable to connect to MPU.\n", __PRETTY_FUNCTION__);
        p->color(CRGB::Red);
    }
}

MPUState MPUController::init()
{
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE

    printf("%s: Initialising MPU using Arduino Wire library.\n", __PRETTY_FUNCTION__);
    Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    printf("%s: Initialising MPU using Fastwire library.\n", __PRETTY_FUNCTION__);
    Fastwire::setup(400, true);
#endif
    this->mpu.initialize();

    if (!(this->mpu.testConnection()))
    {
        // Connection failed.
        return MPUState::ERROR;
    }

    return MPUState::CONNECTED;
}