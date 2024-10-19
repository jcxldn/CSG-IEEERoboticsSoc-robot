#ifndef SRC_DRIVE_H_
#define SRC_DRIVE_H_

// uint8_t
#include <stdint.h>

// QueueHandle_t
#include <Arduino_FreeRTOS.h>
#include <queue.h>

#define PIN_MOTOR_LEFT_ENABLE 5
#define PIN_MOTOR_LEFT_FORWARD 7
#define PIN_MOTOR_LEFT_REVERSE 8

#define PIN_MOTOR_RIGHT_ENABLE 6
#define PIN_MOTOR_RIGHT_FORWARD 11
#define PIN_MOTOR_RIGHT_REVERSE 9

#define PIN_MOTOR_TYPE OUTPUT

#define REQ_TYPE_LEFT 0
#define REQ_TYPE_RIGHT 1

// analogWrite 0-255 (8 bit)
typedef struct
{
    bool side; // 0 left, 1 right
    uint8_t speed;
    bool forward;
    bool reverse;
} control_req_t;

typedef struct
{
    control_req_t left;
    control_req_t right;
} dual_control_req_t;

class Drive
{
private:
    void setup_pins();
    void create_queue();
    void register_task();
    static void handle_req(control_req_t req);

public:
    Drive();
    static void drive_task(void *param);
    static QueueHandle_t xPointerQueue;
};

#endif /* SRC_DRIVE_H_ */