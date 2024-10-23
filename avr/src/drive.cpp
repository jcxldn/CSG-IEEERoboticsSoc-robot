// Arduino Core
#include <Arduino.h>

// FreeRTOS core, task and queue libraries
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include "drive.h"

Drive::Drive()
{
    this->setup_pins();
    bool success = this->create_queue();
    if (success)
    {
        xTaskCreate(this->task, "drived", configMINIMAL_STACK_SIZE, this, configMAX_PRIORITIES, NULL);
    }
}

void Drive::setup_pins()
{
    pinMode(TB6612FNG_PIN_STBY, OUTPUT);
    pinMode(TB6612FNG_PIN_PWMA, OUTPUT);
    pinMode(TB6612FNG_PIN_PWMB, OUTPUT);
    pinMode(TB6612FNG_PIN_AIN1, OUTPUT);
    pinMode(TB6612FNG_PIN_BIN1, OUTPUT);
}

bool Drive::create_queue()
{
    this->xPointerQueue = xQueueCreate(
        // The number of items the queue can hold
        10,
        // Size of each item only big enough for a pointer
        sizeof(drive_req_t *));

    // if null, not created
    // so return true if created sucessfully
    return xPointerQueue != NULL;
}

void Drive::task(void *pvParameters)
{
    Drive *pThis = (Drive *)pvParameters;

    // Create a struct to store the pointer from the object fetched from the queue
    drive_req_t *pxRecievedPointer;

    while (1)
    {
        // This task will be SUSPENDED until data is available
        // ie. other tasks can run on this core
        if (xQueueReceive(pThis->xPointerQueue, &(pxRecievedPointer), portMAX_DELAY) == pdTRUE)
        {
            // We have recieved data
            digitalWrite(pxRecievedPointer->stby, TB6612FNG_PIN_STBY);

            pThis->handle_req(pxRecievedPointer->left);
            pThis->handle_req(pxRecievedPointer->right);

            free(pxRecievedPointer); // is this needed - GC??
        }
    }
}

void Drive::handle_req(channel_req_t req)
{
    int pwm_pin, in1_pin;

    if (req.side == ChannelSide::LEFT)
    {
        // left uses channel B
        pwm_pin = TB6612FNG_PIN_PWMB;
        in1_pin = TB6612FNG_PIN_BIN1;
    }
    else
    {
        // right uses channel A
        pwm_pin = TB6612FNG_PIN_PWMA;
        in1_pin = TB6612FNG_PIN_AIN1;
    }

    digitalWrite(in1_pin, req.direction);
    analogWrite(pwm_pin, req.pwm);
}

void Drive::enqueue(drive_req_t *req)
{
    // Add a pointer to the memory address of the message to the queue.
    // This allows us to add arbitary length datatypes to the queue.
    // Use a tickType of 0 to not block until a space if available.
    // TODO: Add handling for when the queue is full, although may be unlikely to happen.
    xQueueSend(this->xPointerQueue, (void *)&req, (TickType_t)0);
}

// Move both sides/channels at the same speed in the specified direction.
void Drive::queueDualSpeedReq(uint8_t speed, Direction direction)
{
    channel_req_t left = {ChannelSide::LEFT, direction, speed};
    channel_req_t right = {ChannelSide::RIGHT, direction, speed};
    drive_req_t req = {StandbyMode::OFF, left, right};

    this->enqueue(&req);
}

// utility funcs below

void Drive::forward(uint8_t speed)
{
    this->queueDualSpeedReq(speed, Direction::FORWARD);
}

void Drive::reverse(uint8_t speed)
{
    this->queueDualSpeedReq(speed, Direction::BACKWARD);
}

void Drive::brake()
{
    // EITHER "In1 LOW, PWM LOW, STBY HIGH" OR
    //        "In1 HIGH, PWM LOW, STBY HIGH"
    // results in "Short brake" (without PWM action)
    // direction, speed does not matter
    // IN1, IN2, PWM all any, STBY LOW = standby

    channel_req_t left = {ChannelSide::LEFT, Direction::BACKWARD, 0};
    channel_req_t right = {ChannelSide::RIGHT, Direction::BACKWARD, 0};
    drive_req_t req = {StandbyMode::OFF, left, right};
}

void Drive::standby()
{
    // direction, speed does not matter
    // IN1, IN2, PWM all any, STBY LOW = standby

    channel_req_t left = {ChannelSide::LEFT, Direction::BACKWARD, 0};
    channel_req_t right = {ChannelSide::RIGHT, Direction::BACKWARD, 0};
    drive_req_t req = {StandbyMode::ON, left, right};

    this->enqueue(&req);
}