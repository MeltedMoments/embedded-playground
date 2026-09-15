/**
 * climate.h
 */
#pragma once

// Comfort calculation
constexpr float IDEAL_TEMPERATURE   = 23.0;
constexpr float IDEAL_HUMIDITY      = 45.0;
constexpr float TEMPERATURE_PENALTY = 4.0;
constexpr float HUMIDITY_PENALTY    = 0.9;

String comfort_text (float comfort);
float comfort_score(float temperature, float humidity);

// constexpr int LCD_ROWS = 2;
// constexpr int LCD_COLS = 16;
// constexpr int LCD_HEARTBEAT_ROW = 1;
// constexpr int LCD_HEARTBEAT_COL = 15;

// void setup_lcd_display();
// void show_lcd_climate(float temperature, float humidity, float comfort);
// void show_lcd_heartbeat(bool heartbeat_on);
