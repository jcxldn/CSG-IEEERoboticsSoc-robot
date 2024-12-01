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
    this->p = p;
    state = this->initMpu();

    if (state != MPUState::CONNECTED)
    {
        // Show error
        Serial.println(F("ERR INIT"));
        p->color(CRGB::Red);
    }
    else
    {
        // Connected
        state = this->configureMpu();
        if (state != MPUState::READY)
        {

            // Show error
            Serial.println(F("ERR CONF"));
            p->color(CRGB::Red);
        }
    }
}

MPUState MPUController::initMpu()
{
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif
    Serial.println(F("Init MPU..."));
    this->mpu.initialize();

    if (!(this->mpu.testConnection()))
    {
        // Connection failed.
        return MPUState::ERROR_INIT;
    }

    return MPUState::CONNECTED;
}

MPUState MPUController::configureMpu()
{
    // setup based on https://github.com/ElectronicCats/mpu6050/blob/master/examples/MPU6050_DMP6/MPU6050_DMP6.ino

    // Init DMP
    Serial.println(F("Init DMP..."));
    device_state = mpu.dmpInitialize();

    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);

    Serial.println(device_state);

    // Calibration time
    if (device_state == 0)
    {
        p->color(CRGB::Blue);
        // success

        mpu.CalibrateAccel(7); // 6-7 is a good combo
        mpu.CalibrateGyro(7);  // 6-7 is a good combo
        // mpu.PrintActiveOffsets();

        // print active offsets
        mpu.PrintActiveOffsets();

        // enable DMP
        mpu.setDMPEnabled(true);

        // Enable interrupt detection (freertos)
        // N/A here since INT pin is not connected

        packet_size = mpu.dmpGetFIFOPacketSize();
        // fifo_count = mpu.getFIFOCount();

        return MPUState::READY;
    }
    else
    {
        return MPUState::ERROR_CONFIGURE;
    }
};

float MPUController::task()
{
    if (mpu.getFIFOCount() >= packet_size)
    {
        // packet is ready
        if (mpu.dmpGetCurrentFIFOPacket(measurements.FIFOBuffer))
        {
            mpu.dmpGetQuaternion(&measurements.q, measurements.FIFOBuffer);
            mpu.dmpGetGravity(&measurements.gravity, &measurements.q);
            mpu.dmpGetYawPitchRoll(measurements.ypr, &measurements.q, &measurements.gravity);

            float degrees = measurements.ypr[0] * RAD_TO_DEG;
            Serial.println(String(degrees));
            return degrees;
        }
    }
}