#include "led_strip.h"
#include "header.h"

// Define the array of leds
CRGB leds[NUMBER_OF_LEDS];

void led_strip_init()
{
    FastLED.addLeds<NEOPIXEL, LED_STRIP_PIN>(leds, NUMBER_OF_LEDS);
}

void led_strip_set_color(CRGB color)
{
    for (size_t i = 0; i < NUMBER_OF_LEDS; i++) {
        leds[i] = color;
    }
    FastLED.show();
}
