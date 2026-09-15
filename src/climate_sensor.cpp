/** 
 *  climate_sensor.cpp
*/
#include <Adafruit_AHTX0.h>

#include "climate_sensor.h"
#include "config.h"

Adafruit_AHTX0 aht;
bool aht_ready = false;
bool have_climate_reading = false;
unsigned long last_sensor_time = 0;
float current_temperature = 0.0;
float current_humidity = 0.0;

void setup_climate_sensor() {
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setTimeOut(100);
    aht_ready = aht.begin(&Wire);
    if (! aht_ready) {
        Serial.println("AHT20 init failed");
        return;
    }
    Serial.println("AHT20 ready");
}

void read_climate_sensor() {
    unsigned long now = millis();
    if (now - last_sensor_time < SENSOR_INTERVAL_MS) {
        return;
    }

    sensors_event_t humidity;
    sensors_event_t temperature;
    aht.getEvent(&humidity, &temperature);
    current_temperature = temperature.temperature;
    current_humidity = humidity.relative_humidity;
    have_climate_reading = true;
    last_sensor_time = now;
}

