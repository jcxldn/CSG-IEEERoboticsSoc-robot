#ifndef SRC_PIXEL_H_
#define SRC_PIXEL_H_

#include <FastLED.h>

#define PIXEL_NUM 1
#define PIXEL_PIN 4

class Pixel
{
public:
    Pixel();
    void color(CRGB color);

    CRGB leds[PIXEL_NUM];
};

#endif /* SRC_PIXEL_H_ */