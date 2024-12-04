// Arduino Core
#include <Arduino.h>

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
IRState::IRState(ir_channel_t left, ir_channel_t centre, ir_channel_t right)
{
    this->left = left;
    this->centre = centre;
    this->right = right;

    this->visible = (ir_track_parts_visible)(channelOnLine(left, LINE_LEFT) +
                                             channelOnLine(centre, LINE_CENTRE) +
                                             channelOnLine(right, LINE_RIGHT));
}

// return the visible enum (one bit set corresponding to channel)
// to ADD with visible to determine actual
ir_track_parts_visible IRState::channelOnLine(ir_channel_t chan, ir_track_parts_visible bit)
{
    if (chan > BLACK_MIN_THRESHOLD)
    {
        return bit;
    }
    else
    {
        return LINE_NONE;
    }
}