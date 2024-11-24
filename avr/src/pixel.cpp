// Arduino Core
#include <Arduino.h>

#include "pixel.h"

Pixel::Pixel()
{
    FastLED.addLeds<NEOPIXEL, PIXEL_PIN>(this->leds, PIXEL_NUM);
}

void Pixel::color(CRGB color)
{
// QEMU stops (freezes?) at FastLED.show() for some reason, so stub this function if using qemu
#ifdef PLATFORM_TYPE_QEMU
    return;
#endif

    // pixel is available
    // set color
    this->leds[0] = color;
    // display on pixel
    FastLED.show();
}