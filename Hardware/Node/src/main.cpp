#include "ir_sensor.h"
#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    uint16_t distance = ir_read_distance();
    bool evacuated = ir_is_evacuated();
    Serial.println(distance);
    Serial.println(evacuated);
    Serial.println("-----");
    delay(500);
}
