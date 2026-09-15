/**
 * climate_sensor.h
 */
#pragma once

void setup_climate_sensor();
void read_climate_sensor();

// extern Adafruit_AHTX0 aht;
// unsigned long last_sensor_time = 0;
extern bool aht_ready;
extern float current_temperature;
extern float current_humidity;
extern bool have_climate_reading;

