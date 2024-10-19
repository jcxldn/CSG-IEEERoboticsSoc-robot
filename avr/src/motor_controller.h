#ifndef SRC_MOTOR_H_
#define SRC_MOTOR_H_

// uint8_t
#include <stdint.h>

// QueueHandle_t
#include <Arduino_FreeRTOS.h>
#include <queue.h>

#include "drive.h"

class MotorController
{
private:
    static void enqueue(dual_control_req_t *req);

public:
    // valid values: 0-255
    static void fwd(uint8_t speed);

    // valid values: 0-255
    static void rev(uint8_t speed);
};

#endif /* SRC_MOTOR_H_ */