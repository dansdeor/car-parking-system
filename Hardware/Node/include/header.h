/**
 * This is a configuration header file for the node that includes all the necessary config
 * to read from the sensor/camera and send the collected data back to the main system for further processing.
 */
#ifndef NODE_CONFIG
#define NODE_CONFIG

// Node role:
#define GATE // A simple way to burn two different code modifications on different devices

// Node config:
#ifdef GATE
#define NODE_ID "gate"
#else
#define NODE_ID "24"
#endif

// WIFI config:
#define SSID_NAME "ICST"
#define SSID_PASSWORD "arduino123"

// http Server config:
// URL shape of http://IP:PORT/ (the slash at the end is necessary)
#define SERVER_URL "http://132.68.34.22:5000/"

// IR Sensor config:
#define IR_SENSOR_PIN 13

// Camera config:
#define FRAME_SIZE FRAMESIZE_240X240
#define JPEG_QUALITY 10 // 4 to 63 where 4 is the best quality
#define FB_COUNT 1 // frame buffers to allocate
#define BRIGHTNESS 0
#define CONTRAST 2
#define SATURATION 2
#define HORIZONTAL_MIRROR 1
#define VERTICAL_FLIP 1

// Led strip config:
#define LED_STRIP_PIN 12
#define NUMBER_OF_LEDS 4

// Oled display config:
#define I2C_SDA_PIN 15
#define I2C_SCL_PIN 14
#define OLED_CURSOR_X 22
#define OLED_CURSOR_Y 20
#define OLED_TEXT_SIZE 5

#endif