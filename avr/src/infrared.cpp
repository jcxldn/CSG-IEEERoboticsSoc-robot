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

void Infrared::display()
{
    // max (blk) ~700
    // min (wht) ~30
    IRState s = read();
    // printf("[%i,%i,%i]\r\n", s.left, s.centre, s.right);
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

ir_track_status IRState::tick(Drive *drive, Pixel *pixel)
{
    // ir_track_status max = getMax();
    // ir_track_status min = getMin();

    ir_line_tracking_status status;

    ir_channel_t maxValue = max((max(this->left, this->centre)), this->right);
    ir_channel_t minValue = min((min(this->left, this->centre)), this->right);

    if (maxValue < WHITE_MAX_THRESHOLD)
    {
        pixel->color(CRGB::Orange);
        status = ir_line_tracking_status::TRACKING_ALL_WHITE;
    }

    else if (minValue > BLACK_MIN_THRESHOLD)
    {
        pixel->color(CRGB::Blue);
        status = ir_line_tracking_status::TRACKING_ALL_BLACK;
    }
    else
    {
        // TODO: not true shh
        pixel->color(CRGB::Green);
        status = ir_line_tracking_status::TRACKING_ON_LINE;
    }

    switch (status)
    {
    case TRACKING_ON_LINE:
        Serial.println(F("ON_LINE"));
        break;
    case TRACKING_ALL_WHITE:
        Serial.println(F("ALL_WHITE"));
        break;
    case TRACKING_ALL_BLACK:
        Serial.println(F("ALL_BLACK"));
        break;
    }

    switch (status)
    {
    case TRACKING_ALL_WHITE:
        // all white, go forwards
        drive->forward(255);
        break;
    case TRACKING_ALL_BLACK:
        // drive->brake(); // TODO: fix

        drive->brake();
        delay(500);
        drive->turnUntilDegreesRelative(90);
        for (;;)
        {
        }
        break;
    case TRACKING_ON_LINE:
        // think there is a line
        // drive->brake();
        break;
    }
    /**
        // Check if all black
        if (minValue > BLACK_MIN_THRESHOLD)
        {
            Serial.println(F("ALL_BLACK"));
            // all black, turn either direction (right?)
            drive->steer(Direction::FORWARD, 255, 0);

            pixel->color(CRGB::Purple);
        }

        // for third value, get closest to max or min
        if (abs(maxValue) > abs(minValue))
        {
            Serial.println(F("ON_LINE"));
            // on line
            pixel->color(CRGB::Green);
        } */
}