#include "main.h"

#include "drive.h"
#include "infrared.h"
#include "mpu_controller.h"
#include "pid.h"
#include "pid_consumer.h"

#include <Arduino.h>

Drive *drive;
Pixel *pixel;
MPUController *mpu;
Infrared *ir;
PIDConsumer *pidConsumer;
PIDController *pid;

float pidLastMeasured = 0;

boolean isTurning;

float lastTime;

pidconsumer_block_until blockCondition = pidconsumer_block_until::NONE;

// printf
FILE f_out;
int sput(char c, __attribute__((unused)) FILE *f) { return !Serial.write(c); }

void blockUntilOn(ir_track_parts_visible item)
{
  while (ir->read().visible2 & item == 1)
  {
    ;
    ;
  }
  return;
}

void blockUntilOnlyOn(ir_track_parts_visible item)
{
  while (ir->read().visible == item)
  {
    ;
    ;
  }
  return;
}

void blockUntilOff(ir_track_parts_visible item)
{
  while ((ir->read().visible & item == true))
  {
    ;
    ;
  }
  return;
}

// determine speed
int lSpeed, rSpeed;

void setup()
{
  Serial.begin(115200);

  // printf
  fdev_setup_stream(&f_out, sput, nullptr, _FDEV_SETUP_WRITE); // cf https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#gaf41f158c022cbb6203ccd87d27301226
  stdout = &f_out;

  // Wait for serial to be ready.
  while (!Serial)
  {
    ;
  }

  Serial.println(F("Instanciating classes"));

  // Drive drive;
  // mpu = new MPUController(pixel);
  drive = new Drive(mpu);
  pixel = new Pixel();
  ir = new Infrared();
  pidConsumer = new PIDConsumer();
  // was 5,0,40 (default)
  // 30,0,0 leads to +240 (right only), -210 (left only)
  // 30, 0.1, 0.01
  // kp, 0, 1
  // ----
  // kp, 0, 0 | ki <=3
  // 2 okay
  // 3 too much
  // 1?
  pid = new PIDController(pidConsumer->kp, 0, 0);
  // PIDController pid(1.0, 0.1, 0.01);

  pixel->color(CRGB::Orange);

  // wait for button press
  pinMode(2, INPUT);
  while (digitalRead(2) == HIGH)
  {
    delay(100);
  }

  pixel->color(CRGB::Green);
  delay(1000);
}

void loop()
{
  // mpu->task();
  // drive_task();

  // test: read and do pid stuffs
  IRState s = ir->read();

  long avg = 0;
  long sum = 0; // long or too big when all under a line (if int will be over 32767 and overflow)

  avg += s.left * 1 * ANALOGUE_MAX_VALUE;
  avg += s.centre * 2 * ANALOGUE_MAX_VALUE;
  avg += s.right * 3 * ANALOGUE_MAX_VALUE;

  sum += s.left + s.centre + s.right;

  float pidInput = avg / sum;

  float currTime = (float)millis() / (float)1000;
  if (lastTime == 0)
  {
    lastTime = currTime;
  }

  float dt = currTime - lastTime;

  float data = pid->compute(pidInput, GOAL_CENTRE, dt);

  lastTime = currTime;
  pidLastMeasured = pidInput;
  // handling

  if (data > 0)
  {
    int speed = constrain(data, SPEED_ACTIVE_MIN, SPEED_ACTIVE_MAX);
    if (speed < SPEED_DIFF)
    {
      lSpeed = SPEED_ACTIVE_MAX;
      rSpeed = SPEED_ACTIVE_MAX - speed;
    }
    else
    {
      lSpeed = SPEED_ACTIVE_MIN;
      rSpeed = speed;
    }
  }

  else if (data < 0)
  {
    // flip as will be negative
    int speed = constrain(-data, SPEED_ACTIVE_MIN, SPEED_ACTIVE_MAX);

    if (speed < SPEED_DIFF)
    {

      lSpeed = SPEED_ACTIVE_MAX - speed;
      rSpeed = SPEED_ACTIVE_MAX;
    }
    else
    {
      lSpeed = speed;
      rSpeed = SPEED_ACTIVE_MIN;
    }
  }
  else
  {
    // either none on  line or all on line
    // TODO, use PID in gyro to go straight
    lSpeed = SPEED_ACTIVE_MAX;
    rSpeed = SPEED_ACTIVE_MAX;
  }

  printf("Kp: %.5f | %04d, %04d, %04d | Sum: %05ld | PID (i,o) [%6.3f, %6.1f] | Pos: %10s | DT: %0.5f | Speed (l,r) [%03d, %03d]\r\n",
         pidConsumer->kp,
         s.left,
         s.centre,
         s.right,
         avg,
         pidInput,
         data,
         (data > 0 ? "LINE_RIGHT" : (data < 0 ? "LINE_LEFT" : "LINE_NONE")),
         dt,
         lSpeed,
         rSpeed);

  // drive->steer(Direction::FORWARD, lSpeed, rSpeed);
}
void drive_task()
{
  IRState s = ir->read();

  switch (s.visible)
  {
  case LINE_NONE:
    Serial.println(F("LINE_NONE"));
    // all white, go forward
    drive->forward(255);
    break;
  case LINE_LEFT:
    Serial.println(F("LINE_LEFT"));
    // turn right
    isTurning = true;
    drive->steer(Direction::FORWARD, 0, 255);
    blockUntilOn(LINE_CENTRE);
    drive->steer(Direction::FORWARD, 0, 196);
    blockUntilOnlyOn(LINE_LEFT);
    break;
  case LINE_CENTRE:
    Serial.println(F("LINE_CENTRE"));
    // go straight
    drive->forward(255);
    blockUntilOff(LINE_CENTRE);
    break;
  case LINE_RIGHT:
    Serial.println(F("LINE_RIGHT"));
    // turn left
    isTurning = true;
    drive->steer(Direction::FORWARD, 255, 0);
    blockUntilOn(LINE_CENTRE);
    drive->steer(Direction::FORWARD, 196, 0);
    blockUntilOnlyOn(LINE_RIGHT);
    break;
  }

  while (isTurning && (ir->read().visible == LINE_NONE))
  {
  }

  isTurning = false;
}