#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

constexpr int BOOT_BUTTON_PIN = 0;
constexpr int RGB_LED_PIN = 48;
constexpr unsigned long REPORT_INTERVAL_MS = 2000;

int colour_index = 0;
int last_button_state = HIGH;
unsigned long last_report_time = 0;

Adafruit_NeoPixel pixel(
    1, RGB_LED_PIN, NEO_GRB + NEO_KHZ800
);

void show_colour(int index) {
    switch (index) {
        case 0: 
            pixel.setPixelColor(0, pixel.Color(255, 0, 0));
            break;
        case 1:
            pixel.setPixelColor(0, pixel.Color(0, 255, 0));
            break;         
        case 2:
            pixel.setPixelColor(0, pixel.Color(0, 0, 255));
            break;
    }
    pixel.show();
}

void setup() {
    Serial.begin(115200);
    // delay(5000);
    pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);

    pixel.begin();
    pixel.setBrightness(20);
    pixel.setPixelColor(0, pixel.Color(255, 255, 255));
    pixel.clear();
    pixel.show();
    
    Serial.println("Ready");
}


void loop() {
    unsigned long now = millis();
    // Check the button
    int button_state = digitalRead(BOOT_BUTTON_PIN);
    if (last_button_state == HIGH && button_state == LOW) {
        colour_index = (colour_index + 1) % 3;
        show_colour(colour_index);
    }
    last_button_state = button_state;

    // Keep the heartbeat going
    if (now - last_report_time >= REPORT_INTERVAL_MS) {
        Serial.println("Still alive");
        last_report_time = now;
    }
    // delay(20);
}
