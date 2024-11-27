// Arduino Core
#include <Arduino.h>

#include <LibPrintf.h>

#include "infrared.h"

Infrared::Infrared()
{
    // Setup pins
    pinMode(INFRARED_PIN_LEFT, INPUT);
    pinMode(INFRARED_PIN_CENTRE, INPUT);
    pinMode(INFRARED_PIN_RIGHT, INPUT);
}

IRState Infrared::read()
{
    return {analogRead(INFRARED_PIN_LEFT), analogRead(INFRARED_PIN_CENTRE), analogRead(INFRARED_PIN_RIGHT)};
}

void Infrared::display()
{
    // max (blk) ~700
    // min (wht) ~30
    IRState s = read();
    printf("[%i,%i,%i]\r\n", s.left, s.centre, s.right);
    delay(100);
}

IRState::IRState(ir_channel_t left, ir_channel_t centre, ir_channel_t right)
{
    this->left = left;
    this->centre = centre;
    this->right = right;
}

ir_track_status IRState::match(ir_channel_t value)
{
    if (value == this->left)
    {
        return LINE_LEFT;
    }
    else if (value == this->centre)
    {
        return LINE_CENTRE;
    }
    else if (value == this->right)
    {
        return LINE_RIGHT;
    }
    else
    {
        return LINE_NONE;
    }
}

ir_track_status IRState::getMax()
{
    return match(max((max(this->left, this->centre)), this->right));
}

ir_track_status IRState::getMin()
{
    return match(min((min(this->left, this->centre)), this->right));
}

ir_track_status IRState::tick()
{
    ir_track_status max = getMax();
    ir_track_status min = getMin();

    // for third value, get closest to max or min
    if (abs(max) > abs(min))
    {
    }
}