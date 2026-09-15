/** 
 *  lcd_display.cpp
*/
// #include <Adafruit_AHTX0.h>
#include <rgb_lcd.h>

#include "lcd_display.h"
#include "climate.h"
#include "config.h"

rgb_lcd lcd; 

void setup_lcd_display() {
    lcd.begin(LCD_COLS, LCD_ROWS);
    lcd.setCursor(0, 0);
    lcd.print("Grove 16x2 LCD");
    lcd.setCursor(0, 1);
    lcd.print("Ready");
}

void show_lcd_heartbeat(bool heartbeat_on) {
    lcd.setCursor(15, 1);
    lcd.print(heartbeat_on ? '.' : ' ');
}

void show_lcd_climate(float temperature, float humidity) {
    // Line 1
    String buffer = "T:";
    buffer += String(temperature, 1);
    buffer += "C H:";
    buffer += String(humidity, 1);
    buffer += "%";
    lcd.setCursor(0, 0);
    lcd.print(buffer);

    // Line 2 
    float comfort = comfort_score(temperature, humidity);
    buffer = "Comfort: ";
    buffer += comfort_text(comfort);
    lcd.setCursor(0,1);
    lcd.print(buffer);
}
