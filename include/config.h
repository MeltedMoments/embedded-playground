#pragma once
/** 
 * config.h 
 */
// AHT20 and Grove 16x2 LCD 
constexpr int SDA_PIN = 4;
constexpr int SCL_PIN = 6;

// Waveshare OLED 
constexpr int OLED_MOSI = 11;       // SPI MOSI/data
constexpr int OLED_CLK  = 12;       // SPI clock
constexpr int OLED_CS   = 10;       // chip select
constexpr int OLED_DC   = 9;        // data/command
constexpr int OLED_RST  = 8;        // reset

// Sensor update intervals
constexpr unsigned long HEARTBEAT_INTERVAL_MS = 500;
constexpr unsigned long SENSOR_INTERVAL_MS = 2000;
constexpr unsigned long DISPLAY_INTERVAL_MS = 10000;
constexpr unsigned long START_DISPLAY_DELAY_MS = 1000;
