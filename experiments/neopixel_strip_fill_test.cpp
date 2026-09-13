
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

constexpr int PIXEL_PIN = 4;
constexpr int PIXEL_COUNT = 8;

constexpr unsigned long REPORT_INTERVAL_MS = 5000;
constexpr unsigned long SENSOR_INTERVAL_MS = 1500;

Adafruit_NeoPixel strip(
    PIXEL_COUNT,
    PIXEL_PIN,
    NEO_GRB + NEO_KHZ800
);

unsigned long last_report_time = 0;
unsigned long last_sensor_time = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("NeoPixel Strip test");

    strip.begin();
    strip.clear();
    strip.show();
}

void heartbeat() {
    unsigned long now = millis();
    if (now - last_report_time >= REPORT_INTERVAL_MS) {
        Serial.println("Heartbeat");
        last_report_time = now;
    }
}

void lightshow() {
    unsigned long now = millis();
    if (now - last_sensor_time < SENSOR_INTERVAL_MS) {
        return;
    }

    static int level = 0;
    static int direction = 1;

    strip.clear();
    for (int i=0; i < level; i++) {
        strip.setPixelColor(i, strip.Color(0, 255, 255));
        delay(100);
    }

    strip.show();
    level += direction;

    if (level >= PIXEL_COUNT) {
        level = PIXEL_COUNT;
        direction = -1;
    } else if (level <= 0) {
        level = 0;
        direction = 1;
    }
}

void loop() {
    heartbeat();
    lightshow();
}