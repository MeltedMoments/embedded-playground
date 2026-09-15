/**
 * lcd_display.h
 */
#pragma once

constexpr int LCD_ROWS = 2;
constexpr int LCD_COLS = 16;
constexpr int LCD_HEARTBEAT_ROW = 1;
constexpr int LCD_HEARTBEAT_COL = 15;

void setup_lcd_display();
void show_lcd_climate(float temperature, float humidity, float comfort);
void show_lcd_heartbeat(bool heartbeat_on);
