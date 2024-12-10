#include <Arduino.h>

#include "pid_consumer.h"

PIDConsumer::PIDConsumer()
{
    /**
     * Step 1: calculate kp
     *
     * Formula: max speed = max error * kp
     * max speed = 255
     * max value = 2048 (1024*(3-1))
     * max error = 1024 (2048/2)
     *
     *
     * therefore: kp = max_speed / max_error
     */
    kp = (float)(MAX_SPEED * 2) / (float)GOAL;

    Serial.print(F("KP: "));
    Serial.print(kp, 5);
    Serial.println(F(""));
}