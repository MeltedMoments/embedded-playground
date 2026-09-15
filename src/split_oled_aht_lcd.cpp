#include <Arduino.h>
#include <Wire.h>
#include <algorithm>
#include  <Adafruit_GFX.h>
#include  <Adafruit_SH110X.h>

#include "climate_sensor.h"
#include "lcd_display.h"
#include "climate.h"
#include "config.h"

// Hardware 
constexpr int OLED_MOSI = 11;       // SPI MOSI/data
constexpr int OLED_CLK  = 12;       // SPI clock
constexpr int OLED_CS   = 10;       // chip select
constexpr int OLED_DC   = 9;        // data/command
constexpr int OLED_RST  = 8;        // reset

constexpr int OLED_WIDTH = 64;  
constexpr int OLED_HEIGHT  = 128; 

unsigned long last_heartbeat_time = 0;
unsigned long last_display_time = 0;
bool heartbeat_on = false;

Adafruit_SH1107 display(
    OLED_WIDTH,
    OLED_HEIGHT,
    OLED_MOSI,
    OLED_CLK,
    OLED_DC,
    OLED_RST,
    OLED_CS
);

const unsigned char heart_8x8[] PROGMEM = {
    0b01100110,
    0b11111111,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000,
    0b00000000
};

const unsigned char heart_12x12[] PROGMEM = {
    0b00000000, 0b00000000,
    0b00111000, 0b00111000,
    0b01111100, 0b01111100,
    0b11111110, 0b11111110,
    0b11111111, 0b11111110,
    0b11111111, 0b11111110,
    0b01111111, 0b11111100,
    0b00111111, 0b11111000,
    0b00011111, 0b11110000,
    0b00001111, 0b11100000,
    0b00000111, 0b11000000,
    0b00000011, 0b10000000,
    0b00000001, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000
};
// const unsigned char heart_12x12[] PROGMEM = {
//     0b000110001100,
//     0b011110111100,
//     0b011111111111,
//     0b011111111111,
//     0b011111111111,
//     0b011111111111,
//     0b001111111110,
//     0b000111111100,
//     0b000011111000,
//     0b000001110000,
//     0b000000100000,
//     0b000000000000
// };


void init_oled() {
    Serial.println("Starting OLED");
    if (! display.begin(0, true)) {
        Serial.println("OLED init failed");
        return;
    }
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.setRotation(3);
    display.println("Hello world!");
    display.display();
    Serial.println("OLED ready");
}

void setup() {
    Serial.begin(115200);
    delay(3000);
    setup_climate_sensor();
    setup_lcd_display();
    init_oled();
    // Show display 1 sec after startup
    last_display_time = millis() - (DISPLAY_INTERVAL_MS - START_DISPLAY_DELAY_MS);
}


void oled_heartbeat() {
    display.setTextColor(SH110X_WHITE);
    int colour = heartbeat_on ? SH110X_WHITE : SH110X_BLACK;
    // display.fillCircle(120, 5, 3, colour);
    display.drawBitmap(
        112, 2,
        heart_8x8,
        8,8,
        colour
    );
    // display.drawBitmap(
    //     112, 2,
    //     heart_12x12,
    //     16,16,
    //     colour
    // );
    display.display();
}

void heartbeat() {
    unsigned long now = millis();
    if (now - last_heartbeat_time < HEARTBEAT_INTERVAL_MS) {
        return;
    }

    Serial.println("Heartbeat");
    heartbeat_on = ! heartbeat_on;
    show_lcd_heartbeat(heartbeat_on);
    oled_heartbeat();
    last_heartbeat_time = now;
}

void show_oled(float temperature, float humidity, float comfort) {
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    // Temperature
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Temperature");
    display.setTextSize(2);
    display.setCursor(0, 14);
    display.print(temperature, 1);
    display.println("C");

    // Humidity
    display.setTextSize(1);
    display.setCursor(0, 38);
    display.println("Humidity");
    display.setTextSize(2);
    display.setCursor(0, 50);
    display.print(humidity, 1);
    display.println("%");
    
    // Comfort
    display.setTextSize(1);
    display.setCursor(70, 38);
    display.println("Comfort ");
    display.setTextSize(2);
    display.setCursor(70, 50);
    display.println(comfort_text(comfort));
    
    display.display();
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

    float comfort = comfort_score(current_temperature, current_humidity);
    Serial.printf("Temperature %.2f C Humidity %.2f %%, Score: %.2f\n",
        current_temperature,
        current_humidity,
        comfort
    );
    
    show_lcd_climate(current_temperature, current_humidity);
    show_oled(current_temperature, current_humidity, comfort);
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
