#pragma once
/** 
 * config.h 
 */
constexpr int SDA_PIN = 4;
constexpr int SCL_PIN = 6;
// constexpr int LCD_COLS = 16;
// constexpr int LCD_ROWS = 2;

// Sensor intervals
constexpr unsigned long HEARTBEAT_INTERVAL_MS = 1000;
constexpr unsigned long SENSOR_INTERVAL_MS = 2000;
constexpr unsigned long DISPLAY_INTERVAL_MS = 10000;
constexpr unsigned long START_DISPLAY_DELAY_MS = 1000;

// Comfort calculation
// constexpr float IDEAL_TEMPERATURE   = 23.0;
// constexpr float IDEAL_HUMIDITY      = 45.0;
// constexpr float TEMPERATURE_PENALTY = 4.0;
// constexpr float HUMIDITY_PENALTY    = 0.9;
