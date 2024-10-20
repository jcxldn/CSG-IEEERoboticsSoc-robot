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

MPUController::MPUController(Pixel *p)
{
    MPUState state = this->init();

    if (state == MPUState::CONNECTED)
    {
        // Continue
    }
    else
    {
        // Show error
        p->color(CRGB::Red);
    }
}

MPUState MPUController::init()
{
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE

    Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
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