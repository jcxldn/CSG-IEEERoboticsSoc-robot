#include "main.h"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

void setup()
{
  Serial.begin(115200);

  // Wait for serial to be ready.
  while (!Serial)
  {
    ;
  }

  xTaskCreate(sensord, "sensord", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES, NULL);
}

void loop() {} // Empty, place logic in FreeRTOS tasks instead.

void sensord(void *param)
{
  (void)param;

  int value = 0;
  for (;;) // never return or exit, infinite loop
  {
    value = analogRead(A0);
    Serial.print("A0: ");
    Serial.print(value, HEX);
    Serial.print("\n");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}