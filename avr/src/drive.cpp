// Arduino Core
#include <Arduino.h>

// FreeRTOS core, task and queue libraries
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include "drive.h"

/* Declare a variable of type QueueHandle_t to hold the handle of the queue being created. */
QueueHandle_t Drive::xPointerQueue;

Drive::Drive()
{
    this->setup_pins();
    this->create_queue(); // will reg task: todo refactor to be better
}

void Drive::drive_task(void *param)
{
    (void)param; // prevent unused (but required) var warning

    // Create a struct to store the pointer from the object fetched from the queue
    dual_control_req_t *pxRecievedPointer;

    while (1)
    {
        // This task will be SUSPENDED until data is available
        // ie. other tasks can run on this core
        if (xQueueReceive(xPointerQueue, &(pxRecievedPointer), portMAX_DELAY) == pdTRUE)
        {
            // We have recieved data
            pxRecievedPointer->left.side = REQ_TYPE_LEFT;
            pxRecievedPointer->right.side = REQ_TYPE_RIGHT;

            handle_req(pxRecievedPointer->left);
            handle_req(pxRecievedPointer->right);
        }
    }
}

void Drive::setup_pins()
{
    pinMode(PIN_MOTOR_LEFT_ENABLE, PIN_MOTOR_TYPE);
    pinMode(PIN_MOTOR_LEFT_FORWARD, PIN_MOTOR_TYPE);
    pinMode(PIN_MOTOR_LEFT_REVERSE, PIN_MOTOR_TYPE);

    pinMode(PIN_MOTOR_RIGHT_ENABLE, PIN_MOTOR_TYPE);
    pinMode(PIN_MOTOR_RIGHT_FORWARD, PIN_MOTOR_TYPE);
    pinMode(PIN_MOTOR_RIGHT_REVERSE, PIN_MOTOR_TYPE);
}

void Drive::create_queue()
{
    // Create a queue of a fixed size and type
    xPointerQueue = xQueueCreate(
        // The number of items the queue can hold
        10,
        // Size of each item only big enough for a pointer
        sizeof(dual_control_req_t *));

    if (xPointerQueue == NULL) // 0 == NULL
    {
        // Queue not created
        printf("Queue not created.");
    }
    else
    {
        // Queue created successfully
        this->register_task();
    }
}

void Drive::register_task()
{
    xTaskCreate(Drive::drive_task, "drived", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES, NULL);
}

void Drive::handle_req(control_req_t req)
{
    if (req.side == REQ_TYPE_LEFT)
    {
        // left
        analogWrite(PIN_MOTOR_LEFT_ENABLE, req.speed);
        digitalWrite(PIN_MOTOR_LEFT_FORWARD, req.forward);
        digitalWrite(PIN_MOTOR_LEFT_REVERSE, req.reverse);
    }
    else
    {
        // right
        analogWrite(PIN_MOTOR_RIGHT_ENABLE, req.speed);
        digitalWrite(PIN_MOTOR_RIGHT_FORWARD, req.forward);
        digitalWrite(PIN_MOTOR_RIGHT_REVERSE, req.reverse);
    }
}