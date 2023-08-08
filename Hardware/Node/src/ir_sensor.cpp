#include "ir_sensor.h"
#include "config.h"
#define IR_ADC_PIN 4

// Basic function assmbled from the ir datasheet and by experimenting
#define THETA_1 14000
#define THETA_2 800
#define THETA_3 3

#define CONVERT_TO_CM(ANALOG_READ) ((THETA_1 / ((ANALOG_READ)-THETA_2)) + THETA_3)

#define NUMBER_OF_SAMPLES 8
#define TIME_BETWEEN_SAMPLES 20 // In milliseconds
#define DELTA 500

#define EVACUATED_DISTANCE_THRESHOLD 100
static bool is_evacuated = false;

uint16_t ir_read_distance()
{
    uint16_t avg_sample, sum_samples;
    uint16_t first_sample, current_sample;

    first_sample = analogRead(IR_ADC_PIN);
    sum_samples = first_sample;

    for (uint8_t i = 0; i < NUMBER_OF_SAMPLES - 1; i++) {
        delay(TIME_BETWEEN_SAMPLES);
        current_sample = analogRead(IR_ADC_PIN);

        if (abs(first_sample - current_sample) > DELTA) {
            return 0;
        }
        sum_samples += current_sample;
    }

    avg_sample = CONVERT_TO_CM(sum_samples / NUMBER_OF_SAMPLES);
    is_evacuated = (avg_sample > EVACUATED_DISTANCE_THRESHOLD);

    return avg_sample;
}

bool ir_is_evacuated()
{
    return is_evacuated;
}
