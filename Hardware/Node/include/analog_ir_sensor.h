#ifndef ANALOG_IR_SENSOR_HEADER
#define ANALOG_IR_SENSOR_HEADER
#include <Arduino.h>

/**
 * Brief: return the distance read from the IR sensor
 * Return: distance in cm or 0 if there was read error
 */
uint16_t ir_read_distance();

/**
 * Brief: Checks whether the measured space is free of objects
 * IT MUST BE CALLED AFTER CALLING ir_read_distance()
 * Return: true if no objects in field of view else flase
 */
bool ir_is_evacuated();

bool analog_ir_object_detected();


/** example
    Serial.print("Measured distance: ");
    Serial.println(distance);
    Serial.print("Is evacuated: ");
    Serial.println(evacuated);
    Serial.println("\n");
*/

#endif