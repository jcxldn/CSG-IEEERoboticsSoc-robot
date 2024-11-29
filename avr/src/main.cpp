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
  mpu = new MPUController(pixel);
  drive = new Drive(mpu);
  pixel = new Pixel();
  ir = new Infrared();
  // PIDController pid(1.0, 0.1, 0.01);

  pixel->color(CRGB::Green);
}

void loop()
{
  // mpu->task();
  drive_task();
}

void drive_task()
{
  // Read IR sensors
  // ir->display();
  IRState s = ir->read();
  // printf("[%i,%i,%i]\r\n", s.left, s.centre, s.right);
  s.tick(drive, pixel);
  // drive->turnUntilDegreesAbsolute(200, Direction::FORWARD, 255, 255);
}