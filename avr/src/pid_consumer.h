#ifndef SRC_PID_CONSUMER_H_
#define SRC_PID_CONSUMER_H_

#define NUM_IR_SENSORS 3
#define ANALOGUE_BITS 10
#define ANALOGUE_MAX_VALUE (2 ^ ANALOGUE_BITS)

// #define MAX_VALUE ((ANALOGUE_MAX_VALUE) * (NUM_IR_SENSORS))
#define GOAL 23000 // middle sensor max
#define MAX_SPEED 255

#define SPEED_INACTIVE 0
#define SPEED_ACTIVE_MIN 25
#define SPEED_ACTIVE_MAX 255

class PIDConsumer
{
public:
    float kp;
    PIDConsumer();
};

#endif /* SRC_PID_CONSUMER_H_ */