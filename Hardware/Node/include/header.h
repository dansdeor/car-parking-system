/**
 * This is a configuration header file for the node that includes all the necessary data.
 * To read from the sensor and send the data back to the main system for further processing.
 */

#ifndef NODE_CONFIG
#define NODE_CONFIG

// WIFI config:
#define SSID_NAME "car-parking"
#define SSID_PASSWORD "12345678"

// http Server config:
// URL shape of http://IP:PORT/
#define SERVER_URL "http://192.168.1.57:5000"

// Node details:
#define NODE_ID "node"

// IR Sensor details:
#define IR_SENSOR_PIN 2

// Camera details:

#endif
