#ifndef CAMERA_HEADER
#define CAMERA_HEADER
#include <Arduino.h>

void camera_init();

bool camera_get_base64_image(String& encoded_base64_image);

#endif