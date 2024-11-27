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
  drive = new Drive();
  pixel = new Pixel();
  mpu = new MPUController(pixel);
  ir = new Infrared();
  // PIDController pid(1.0, 0.1, 0.01);

  pixel->color(CRGB::Green);
}

void loop()
{
  mpu->task();
  ir->display();
  drive_task();
}

void drive_task()
{
  Serial.println(F("drive_task"));
}