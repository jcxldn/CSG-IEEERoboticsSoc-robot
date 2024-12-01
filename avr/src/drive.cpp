// Arduino Core
#include <Arduino.h>

#include "drive.h"

#include <LibPrintf.h>

Drive::Drive(MPUController *mpu)
{
    this->mpu = mpu;

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

// -179.99 -> 180
int wrap(int direction)
{
    while (direction < -179.99)
        direction += (180 * 2) - 0.01;
    while (direction > 180)
        direction -= (180 * 2) - 0.01;
    return direction;
}

// blocking
void Drive::turnUntilDegreesRelative(float deg)
{
    float currentAngle = mpu->task();

    float targetAngle = wrap(currentAngle + deg);

    // printf("%f -> %f (deg: %f)\r\n", currentAngle, targetAngle, deg);

    float offsetToTarget = targetAngle - currentAngle;

    float initialOffset = offsetToTarget;

    while (abs(offsetToTarget) > 1) // less leads to "wobble" rn
    {
        float scalingPercentage = max(offsetToTarget / initialOffset, 0.5);
        Serial.print(F("[LOOP] "));

        // -deg means turn left
        if (offsetToTarget > 0)
        {
            // turning right

            // 255,25 -> 1.76 out
            Serial.print(F("(ACTION: RIGHT) "));
            steer(Direction::FORWARD, (int)(255 * scalingPercentage), 25);
        }
        else
        {
            // turning left
            Serial.println(F("(ACTION: LEFT)"));
            steer(Direction::FORWARD, 25, (int)(255 * scalingPercentage));
        }
        currentAngle = mpu->task();

        offsetToTarget = targetAngle - currentAngle;

        Serial.print(F("[CURR_ANGLE: "));
        Serial.print(currentAngle);
        Serial.print("] ");

        Serial.print(F("[OFF_TO_TARGET: "));
        Serial.print(offsetToTarget);
        Serial.print("] ");

        Serial.print(F("[STEER: "));
        Serial.print((int)(255 * scalingPercentage));
        Serial.print("] ");

        Serial.print(F("[SCALING: "));
        Serial.print(scalingPercentage);
        Serial.print("] \r\n");
    }

    brake();
}