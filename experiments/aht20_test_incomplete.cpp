#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

constexpr int SDA_PIN = 8;
constexpr int SCL_PIN = 9;
constexpr unsigned long REPORT_INTERVAL_MS = 2000;

unsigned long last_report_time = 0;
Adafruit_AHTX0 aht;
bool aht_ready = false;

bool wake_aht20() {
    for (int address = 1; address < 127; address++){
        Wire.beginTransmission(address);
        int error = Wire.endTransmission();
        if (error == 0) {
            Serial.printf("Found a device at 0x%02X\n", address);
            return true;
        }
    }
    return false;
}

void setup() {
    Serial.begin(115200);
    delay(12000);

    Serial.println("Starting I2C");
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(50000);   // 50 kHz
    Wire.setTimeOut(100);
    // Wire.beginTransmission(0x38);
    // int error = Wire.endTransmission();
    // Serial.printf("Probe result %d\n", error);
    delay(500);

    Serial.printf(
        "SDA: %d SCL %d\n",
        digitalRead(SDA_PIN),
        digitalRead(SCL_PIN)
    );

    if (! wake_aht20()) {
            Serial.println("No I2C device found");
            return;
    }

    Serial.println("Trying AHT20");
    aht_ready = aht.begin(&Wire);
    if (! aht_ready) {
        Serial.println("AHT20 init failed");
        return;
    }
    Serial.println("AHT20 ready");
}

void heartbeat() {
    unsigned long now = millis();
    // Keep the heartbeat going
    if (now - last_report_time >= REPORT_INTERVAL_MS) {
        Serial.println("Heartbeat");
        last_report_time = now;
    }
}

void loop() {
    heartbeat();
    if (! aht_ready) {
        delay(1000);
        return;
    }
    sensors_event_t humidity;
    sensors_event_t temperature;

    aht.getEvent(&humidity, &temperature);
    Serial.printf("Temperature %.2f C Humidity %.2f %%\n",
        temperature.temperature,
        humidity.relative_humidity
    );
    // Serial.println("Heartbeat");
    delay(2000);
}


    // unsigned long now = millis();
    // // Check the button
    // int button_state = digitalRead(BOOT_BUTTON_PIN);
    // if (last_button_state == HIGH && button_state == LOW) {
    //     colour_index = (colour_index + 1) % 3;
    //     show_colour(colour_index);
    // }
    // last_button_state = button_state;

    // // Keep the heartbeat going
    // if (now - last_report_time >= REPORT_INTERVAL_MS) {
    //     Serial.println("Still alive");
    //     last_report_time = now;
    // }
