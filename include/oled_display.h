/**
 * oled_display.h
 */
#pragma once

constexpr int OLED_WIDTH = 64;  
constexpr int OLED_HEIGHT = 128; 
constexpr int OLED_HEARTBEAT_ROW = 2;
constexpr int OLED_HEARTBEAT_COL = 115;

void setup_oled_display();
void show_oled_climate(float temperature, float humidity);
void show_oled_heartbeat(bool heartbeat_on);

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
