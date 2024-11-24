#include "main.h"

#include "drive.h"
#include "mpu_controller.h"
#include "pid.h"

#include <Arduino.h>

Drive *drive;
Pixel *pixel;
MPUController *mpu;

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
  // PIDController pid(1.0, 0.1, 0.01);

  pixel->color(CRGB::Green);
}

void loop()
{
  mpu->task();
  // drive_test();
}

void drive_test()
{
  Serial.println("DRIVE TEST");
  drive->forward(255);
  delay(500);
  drive->standby();
  delay(500);
  drive->reverse(255);
  delay(500);
  drive->standby();
  delay(500);
}