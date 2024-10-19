// Arduino Core
#include <Arduino.h>

// https://github.com/embeddedartistry/arduino-printf - printf()
#include <LibPrintf.h>

#include "pixel.h"

Pixel::Pixel()
{
    printf("%s: Initialising %i pixel(s).\n", __PRETTY_FUNCTION__, PIXEL_NUM);
    this->xLedMutex = xSemaphoreCreateMutex();
    FastLED.addLeds<NEOPIXEL, PIXEL_PIN>(this->leds, PIXEL_NUM);
}

void Pixel::color(CRGB color)
{
    // Take the mutex, which signals the pixel as unavailable
    // If the mutex is in use, this will **block** until it is available again.
    xSemaphoreTake(this->xLedMutex, portMAX_DELAY);

    // pixel is available
    // set color
    this->leds[0] = color;
    // display on pixel
    FastLED.show();

    // Hand back the mutex to signal the Pixel's availability to other tasks.
    xSemaphoreGive(this->xLedMutex);
}