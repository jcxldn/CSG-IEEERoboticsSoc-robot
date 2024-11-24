#ifndef SRC_DRIVE_H_
#define SRC_DRIVE_H_

// uint8_t
#include <stdint.h>

// Left uses channel B
// Right uses channel A

#define TB6612FNG_PIN_STBY 3
#define TB6612FNG_PIN_PWMA 5
#define TB6612FNG_PIN_PWMB 6
#define TB6612FNG_PIN_AIN1 7
// AIN2 = !AIN1 (wired via SN74LVC2G14 inverter)
#define TB6612FNG_PIN_BIN1 8
// BIN2 = !BIN1 (wired via SN74LVC2G14 inverter)

#define PWM_NO_BITS 8

#define PWM_MIN_VALUE 0
#define PWM_MAX_VALUE (2 ^ PWM_NO_BITS) - 1 // 255 for 8 bits

// 0 (LOW) - standby ON [TB6612FNG p.4]
enum StandbyMode
{
    ON, // when pin is LOW, standby is ON
    OFF // when pin in HIGH, standby is OFF
};

enum ChannelSide
{
    LEFT,
    RIGHT
};

// might be backwards.
enum Direction
{
    BACKWARD, // In1=0, (In2=1 due to inverter)
    FORWARD   // In1=1 (In2=0 due to inverter)
};

typedef struct
{
    ChannelSide side;
    Direction direction;
    uint8_t pwm; // 0-255 (8-bit)
} channel_req_t;

typedef struct
{
    StandbyMode stby;
    channel_req_t left;
    channel_req_t right;
} drive_req_t;

class Drive
{
private:
    void request(drive_req_t *req);
    void handle_channel_req(channel_req_t req);

public:
    Drive();

    // control functions
    void brake();
    void standby();
    void steer(Direction direction, uint8_t leftSpeed, uint8_t rightSpeed, StandbyMode stbyMode = StandbyMode::OFF);

    // utility shorthand functions
    void forward(uint8_t speed);
    void reverse(uint8_t speed);
};

#endif /* SRC_DRIVE_H_ */