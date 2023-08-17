#ifndef OLED_DISPLAY_HEADER
#define OLED_DISPLAY_HEADER
#include <Arduino.h>

void oled_display_init();

void oled_display_show(String message);

void oled_display_clear();

#endif