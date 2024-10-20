#include "main.h"

#include "drive.h"
#include "motor_controller.h"
#include "mpu_controller.h"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

// https://github.com/embeddedartistry/arduino-printf - printf()
#include <LibPrintf.h>

void setup()
{
  Serial.begin(115200);

  // Wait for serial to be ready.
  while (!Serial)
  {
    ;
  }

#ifdef PLATFORM_TYPE_QEMU
  Serial.println("Using QEMU platform.");
#endif
#ifdef PLATFORM_TYPE_AVR
  Serial.println("Using AVR platform.");
#endif

  printf("%s: Instanciating classes\n\r", __PRETTY_FUNCTION__);

  Drive drive;
  Pixel p;
  MPUController mpu(&p);

  printf("%s: Registering tasks\n\r", __PRETTY_FUNCTION__);

  xTaskCreate(sensord, "sensord", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES, NULL);
}

void loop() {} // Empty, place logic in FreeRTOS tasks instead.

void sensord(void *param)
{
  (void)param;

  for (;;) // never return or exit, infinite loop
  {
    MotorController::fwd(255);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    MotorController::fwd(0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    MotorController::rev(255);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    MotorController::fwd(0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}