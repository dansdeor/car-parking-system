#include "binary_ir_sensor.h"
#include "header.h"
#include <Arduino.h>

void binary_ir_sensor_init()
{
    pinMode(IR_SENSOR_PIN, INPUT);
}

bool binary_ir_object_detected()
{
    return digitalRead(IR_SENSOR_PIN) == 0;
}