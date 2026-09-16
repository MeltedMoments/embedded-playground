// heartbeat.cpp
#include <Arduino.h>
#include "config.h"

unsigned long last_heartbeat_time = 0;
bool current_heartbeat_state = false;

// bool heartbeat_on = false;

void setup_heartbeat() {
    last_heartbeat_time = millis();
}

bool update_heartbeat() {
    unsigned long now = millis();
    if (now - last_heartbeat_time < HEARTBEAT_INTERVAL_MS) {
        return false;
    }

    current_heartbeat_state = ! current_heartbeat_state;
    last_heartbeat_time = now;
    
    return true;
}

bool heartbeat_state() {
    return current_heartbeat_state;
}
