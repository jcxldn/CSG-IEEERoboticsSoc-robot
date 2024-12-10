#ifndef SRC_PID_CONSUMER_H_
#define SRC_PID_CONSUMER_H_

#define NUM_IR_SENSORS 3
#define ANALOGUE_BITS 10
#define ANALOGUE_MAX_VALUE (2 ^ ANALOGUE_BITS)

// #define MAX_VALUE ((ANALOGUE_MAX_VALUE) * (NUM_IR_SENSORS))
#define GOAL 15 // middle sensor max
#define MAX_SPEED 255

#define GOAL_CENTRE 15
#define GOAL_LEFT 9
#define GOAL_RIGHT 20

#define SPEED_INACTIVE 0
#define SPEED_ACTIVE_MIN 0 // was 25 (unable to move on batt)
#define SPEED_ACTIVE_MAX 255

#define SPEED_DIFF 102 // >64, <127

enum pidconsumer_block_until
{
    NONE = 0,
    NOT_POSITIVE = 1,
    NOT_NEGATIVE = 2
};

enum pidconsumer_set_bits
{
    PC_NONE = 0b0000,
    PC_LEFT = 0b0100,
    PC_CENTRE = 0b0010,
    PC_RIGHT = 0b0001,
    PC_ALL = PC_LEFT | PC_CENTRE | PC_RIGHT,
};

class PIDConsumer
{
public:
    float kp;
    PIDConsumer();
};

#endif /* SRC_PID_CONSUMER_H_ */