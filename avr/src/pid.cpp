#include "pid.h"

PIDController::PIDController(float kp, float ki, float kd) : kp_(kp), ki_(ki), kd_(kd), prev_error_(0), integral_(0) {}

float PIDController::compute(float setpoint, float measured_angle, float dt) 
{
    float error = setpoint - measured_angle;
    integral_ += error * dt;
    float derivative = (error - prev_error_) / dt;
    float output = kp_ * error + ki_ * integral_ + kd_ * derivative;
    prev_error_ = error;
    return output;
    }

// Example usage
// PIDController pid(1.0, 0.1, 0.01);
// float output_angle = pid.compute(desired_angle, current_angle, delta_time);