#include "motor_controller.h"

void MotorController::enqueue(dual_control_req_t *req)
{
    QueueHandle_t queue = Drive::xPointerQueue;

    // Add a pointer to the memory address of the message to the queue.
    // This allows us to add arbitary length datatypes to the queue.
    // Use a tickType of 0 to not block until a space if available.
    // TODO: Add handling for when the queue is full, although may be unlikely to happen.
    xQueueSend(queue, (void *)&req, (TickType_t)0);
}

void MotorController::fwd(uint8_t speed)
{
    dual_control_req_t req;

    control_req_t left;
    left.side = 0;
    left.speed = speed;
    left.forward = 1;
    left.reverse = 0;

    control_req_t right;
    right.side = 0;
    right.speed = speed;
    right.forward = 1;
    right.reverse = 0;

    req.left = left;
    req.right = right;

    enqueue(&req);
}

void MotorController::rev(uint8_t speed)
{
    dual_control_req_t req;

    control_req_t left;
    left.side = 0;
    left.speed = speed;
    left.forward = 0;
    left.reverse = 1;

    control_req_t right;
    right.side = 0;
    right.speed = speed;
    right.forward = 0;
    right.reverse = 1;

    req.left = left;
    req.right = right;

    enqueue(&req);
}