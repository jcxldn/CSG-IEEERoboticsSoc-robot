// Arduino Core
#include <Arduino.h>

#include "drive.h"

Drive::Drive()
{
    // Setup pins
    pinMode(TB6612FNG_PIN_STBY, OUTPUT);
    pinMode(TB6612FNG_PIN_PWMA, OUTPUT);
    pinMode(TB6612FNG_PIN_PWMB, OUTPUT);
    pinMode(TB6612FNG_PIN_AIN1, OUTPUT);
    pinMode(TB6612FNG_PIN_BIN1, OUTPUT);
}

void Drive::handle_channel_req(channel_req_t req)
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

void Drive::request(drive_req_t *req)
{
    digitalWrite(TB6612FNG_PIN_STBY, req->stby);

    handle_channel_req(req->left);
    handle_channel_req(req->right);
}

void Drive::steer(Direction direction, uint8_t leftSpeed, uint8_t rightSpeed, StandbyMode stbyMode)
{
    channel_req_t left = {ChannelSide::LEFT, direction, leftSpeed};
    channel_req_t right = {ChannelSide::RIGHT, direction, rightSpeed};
    drive_req_t req = {stbyMode, left, right};
    this->request(&req);
}

// utility funcs below

void Drive::brake()
{
    // EITHER "In1 LOW, PWM LOW, STBY HIGH" OR
    //        "In1 HIGH, PWM LOW, STBY HIGH"
    // results in "Short brake" (without PWM action)
    // direction, speed does not matter
    // IN1, IN2, PWM all any, STBY LOW = standby

    this->steer(
        Direction::BACKWARD, // can be either
        0,                   // left speed
        0                    // right speed
        // StandbyMode defaults to StandbyMode::OFF
    );
}

void Drive::standby()
{
    // direction, speed does not matter
    // IN1, IN2, PWM all any, STBY LOW = standby

    this->steer(
        Direction::BACKWARD, // can be either
        0,                   // left speed
        0,                   // right speed
        StandbyMode::ON      // Output will be high impedance, see datasheet.
    );
}

void Drive::forward(uint8_t speed)
{
    this->steer(Direction::FORWARD, speed, speed);
}

void Drive::reverse(uint8_t speed)
{
    this->steer(Direction::BACKWARD, speed, speed);
}
