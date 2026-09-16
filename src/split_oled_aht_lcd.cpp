#include <Arduino.h>

#include "climate_sensor.h"
#include "lcd_display.h"
#include "oled_display.h"
#include "climate.h"
#include "heartbeat.h"
#include "config.h"

unsigned long last_display_time = 0;

void setup() {
    Serial.begin(115200);

    setup_heartbeat();
    setup_climate_sensor();
    setup_lcd_display();
    setup_oled_display();
    
    // Show display 1 sec after startup
    last_display_time = millis() - (DISPLAY_INTERVAL_MS - START_DISPLAY_DELAY_MS);
}

void heartbeat() {
    if (update_heartbeat()) {
        show_lcd_heartbeat (heartbeat_state());
        show_oled_heartbeat(heartbeat_state());
    }
}

void update_display() {
    unsigned long now = millis();
    if (now - last_display_time < DISPLAY_INTERVAL_MS) {
        return;
    }

    if (! have_climate_reading) {
        Serial.println("No reading");
        return;
    }

    Serial.printf("Temperature %.2f C Humidity %.2f %%\n",
        current_temperature,
        current_humidity
    );
    
    show_lcd_climate(current_temperature, current_humidity);
    show_oled_climate(current_temperature, current_humidity);
    last_display_time = now;
}

void loop() {
    if (! aht_ready) {
        delay(1000);
        return;
    }
    heartbeat();
    read_climate_sensor();
    update_display();
}
