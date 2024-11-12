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
    MPUState state = this->init();

    if (state == MPUState::CONNECTED)
    {
        // 64+32
        // 0, 32, 96, 64
        // today: 0 (gyro sens), 32

        // xTaskCreate(this->task, "mpud", configMINIMAL_STACK_SIZE + 96, this, configMAX_PRIORITIES, NULL);
    }
    else
    {
        // Show error
        Serial.println(F("ERR INIT"));
        p->color(CRGB::Red);
    }
}

MPUState MPUController::init()
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
        return MPUState::ERROR;
    }

    return MPUState::CONNECTED;
}

void MPUController::task(void *pvParameters)
{
    MPUController *pThis = (MPUController *)pvParameters;

    // setup based on https://github.com/ElectronicCats/mpu6050/blob/master/examples/MPU6050_DMP6/MPU6050_DMP6.ino

    // Init DMP
    Serial.println(F("Init DMP..."));
    pThis->device_state = pThis->mpu.dmpInitialize();

    pThis->mpu.setXGyroOffset(0);
    pThis->mpu.setYGyroOffset(0);
    pThis->mpu.setZGyroOffset(0);
    pThis->mpu.setXAccelOffset(0);
    pThis->mpu.setYAccelOffset(0);
    pThis->mpu.setZAccelOffset(0);

    Serial.println(pThis->device_state);

    // Calibration time
    if (pThis->device_state == 0)
    {
        Serial.println(F("CALIBRATING"));
        // success

        pThis->mpu.CalibrateAccel(7); // 6-7 is a good combo
        pThis->mpu.CalibrateGyro(7);  // 6-7 is a good combo
        // pThis->mpu.PrintActiveOffsets();

        // print active offsets

        // enable DMP
        pThis->mpu.setDMPEnabled(true);

        // Enable interrupt detection (freertos)
        // N/A here since INT pin is not connected

        pThis->packet_size = pThis->mpu.dmpGetFIFOPacketSize();
        pThis->fifo_count = pThis->mpu.getFIFOCount();

        uint8_t FIFOBuffer[64];
        Quaternion q; // [w, x, y, z]         Quaternion container
        // VectorInt16 aa;      // [x, y, z]            Accel sensor measurements
        // VectorInt16 gy;      // [x, y, z]            Gyro sensor measurements
        // VectorInt16 aaReal;  // [x, y, z]            Gravity-free accel sensor measurements
        // VectorInt16 aaWorld; // [x, y, z]            World-frame accel sensor measurements
        VectorFloat gravity; // [x, y, z]            Gravity vector
        float ypr[3];        // [yaw, pitch, roll]   Yaw/Pitch/Roll container and gravity vector

        while (1)
        {
            pThis->fifo_count = pThis->mpu.getFIFOCount();

            if (pThis->fifo_count < pThis->packet_size)
            {
            }

            // packet is ready
            if (pThis->mpu.dmpGetCurrentFIFOPacket(FIFOBuffer))
            {

                pThis->mpu.dmpGetQuaternion(&q, FIFOBuffer);
                pThis->mpu.dmpGetGravity(&gravity, &q);
                pThis->mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

                String a = String(ypr[0] * RAD_TO_DEG, 4);
                Serial.println(a);

                // vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
    }
    else
    {
        // Show error
        pThis->p->color(CRGB::Green);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}