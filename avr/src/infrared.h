#ifndef SRC_INFRARED_H_
#define SRC_INFRARED_H_

#define INFRARED_PIN_LEFT A2
#define INFRARED_PIN_CENTRE A1
#define INFRARED_PIN_RIGHT A0

#define WHITE_MAX_THRESHOLD 150
#define BLACK_MIN_THRESHOLD 450

// uint8_t
#include <stdint.h>

#include "drive.h"
#include "pixel.h"

typedef uint16_t ir_channel_t;

enum ir_track_status
{
    LINE_NONE = 0b0000,
    LINE_LEFT = 0b0001,
    LINE_CENTRE = 0b0010,
    LINE_RIGHT = 0b0100,
    LINE_LEFT_CENTRE = LINE_LEFT | LINE_CENTRE,
    LINE_LEFT_CENTRE_RIGHT = LINE_LEFT | LINE_CENTRE | LINE_RIGHT,
    LINE_CENTRE_RIGHT = LINE_CENTRE | LINE_RIGHT,
};

enum ir_line_tracking_status
{
    TRACKING_ON_LINE,
    TRACKING_ALL_BLACK,
    TRACKING_ALL_WHITE
};

class IRState
{
public:
    ir_channel_t left;
    ir_channel_t centre;
    ir_channel_t right;

    ir_track_status match(ir_channel_t value);
    ir_track_status getMax();
    ir_track_status getMin();

    // public:
    IRState(ir_channel_t left, ir_channel_t centre, ir_channel_t right);
    ir_track_status tick(Drive *drive, Pixel *pixel);
};

class Infrared
{
public:
    Infrared();
    IRState read();
    void display();
};

#endif /* SRC_INFRARED_H_ */