#include "main.h"

#include "drive.h"
#include "infrared.h"
#include "mpu_controller.h"
#include "pid.h"

#include <Arduino.h>

Drive *drive;
Pixel *pixel;
MPUController *mpu;
Infrared *ir;

boolean isTurning;

void setup()
{
  Serial.begin(115200);

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
  drive_task();
}

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