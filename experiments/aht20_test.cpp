#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

constexpr int SDA_PIN = 4;
constexpr int SCL_PIN = 6;
// constexpr u_int8_t AHT20_ADDR = 0x38;

constexpr unsigned long REPORT_INTERVAL_MS = 5000;
constexpr unsigned long SENSOR_INTERVAL_MS = 2000;

unsigned long last_report_time = 0;
unsigned long last_sensor_time = 0;
bool aht_ready = false;
Adafruit_AHTX0 aht;

void setup() {
    Serial.begin(115200);
    Serial.println("HELLO!");
    // delay(5000);

    Wire.begin(SDA_PIN, SCL_PIN);
    aht_ready = aht.begin(&Wire);
    if (! aht_ready) {
        Serial.println("AHT20 init failed");
        return;
    }
    Serial.println("AHT20 ready");
}

void heartbeat() {
    unsigned long now = millis();
    if (now - last_report_time >= REPORT_INTERVAL_MS) {
        Serial.println("Heartbeat");
        last_report_time = now;
    }
}

void read_sensor() {
    unsigned long now = millis();
    if (now - last_sensor_time >= SENSOR_INTERVAL_MS) {
        sensors_event_t humidity;
        sensors_event_t temperature;

        aht.getEvent(&humidity, &temperature);
        Serial.printf("Temperature %.2f C Humidity %.2f %%\n",
            temperature.temperature,
            humidity.relative_humidity
        );
        last_sensor_time = now;
    }
}

void loop() {
    if (! aht_ready) {
        delay(1000);
        return;
    }
    heartbeat();
    read_sensor();
}


// bool wake_aht20() {
//     for (int address = 1; address < 127; address++){
//         Wire.beginTransmission(address);
//         int error = Wire.endTransmission();
//         if (error == 0) {
//             Serial.printf("Found a device at 0x%02X\n", address);
//             return true;
//         }
//     }
//     return false;
// }

    // sensors_event_t humidity;
    // sensors_event_t temperature;

    // aht.getEvent(&humidity, &temperature);
    // Serial.printf("Temperature %.2f C Humidity %.2f %%\n",
    //     temperature.temperature,
    //     humidity.relative_humidity
    // );
    // // Serial.println("Heartbeat");
    // delay(2000);


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


//     u_int8_t read_status() {
//     Wire.beginTransmission(AHT20_ADDR);
//     Wire.write(0x71);
//     int error = Wire.endTransmission();
//     if (error != 0) {
//         Serial.printf("Status command failed, error: %d\n", error);
//         return 0xFF;
//     }

//     if (Wire.requestFrom(AHT20_ADDR, (uint8_t)1) != 1) {
//         Serial.println("Read command failed");
//         return 0xFF;
//     }

//     return Wire.read();
// }

// bool trigger_measurement() {
//     Wire.beginTransmission(AHT20_ADDR);
//     Wire.write(0xAC);
//     Wire.write(0x33);
//     Wire.write(0x00);
//     return Wire.endTransmission() == 0;
// }

