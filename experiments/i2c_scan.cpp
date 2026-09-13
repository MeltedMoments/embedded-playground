#include <Arduino.h>
#include <Wire.h>

constexpr int SDA_PIN = 8;
constexpr int SCL_PIN = 9;

void setup() {
    Serial.begin(115200);
    delay(10000);

    Serial.println("Starting I2C");
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setTimeOut(100);

    Serial.println("Scanning I2C bus....");

    for (int address = 1; address < 127; address++){
        // ATM the following seems needed to slow the thing down enough to see the output
        Serial.printf("Trying 0x%02X...", address);
        Wire.beginTransmission(address);
        int error = Wire.endTransmission();
        Serial.printf("error: %d\n", error);
        if (error == 0) {
            Serial.printf(">>>>>>>>>> Found a device at 0x%02X\n", address);
        }
    }
    Serial.println("Scan complete");
}

void loop() {
    Serial.println("Heartbeat");
    delay(1000);
}
