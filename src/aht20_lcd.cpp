#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>
#include <Adafruit_AHTX0.h>


constexpr int SDA_PIN = 4;
constexpr int SCL_PIN = 6;
constexpr int LCD_COLS = 16;
constexpr int LCD_ROWS = 2;

constexpr unsigned long REPORT_INTERVAL_MS = 5000;
constexpr unsigned long SENSOR_INTERVAL_MS = 2000;
constexpr unsigned long DISPLAY_INTERVAL_MS = 10000;

unsigned long last_report_time = 0;
unsigned long last_sensor_time = 0;
unsigned long last_display_time = 0;

bool aht_ready = false;
Adafruit_AHTX0 aht;

rgb_lcd lcd; 

void setup() {
    Serial.begin(115200);
    delay(3000);

    Serial.println("Starting I2C");

    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setTimeOut(100);
    aht_ready = aht.begin(&Wire);
    if (! aht_ready) {
        Serial.println("AHT20 init failed");
        return;
    }
    Serial.println("AHT20 ready");

    lcd.begin(LCD_COLS, LCD_ROWS);

    lcd.setCursor(0, 0);
    lcd.print("Hello ESP");

    lcd.setCursor(0, 1);
    lcd.print("Ready");
}


void heartbeat() {
    unsigned long now = millis();
    if (now - last_report_time >= REPORT_INTERVAL_MS) {
        Serial.println("Heartbeat");
        last_report_time = now;
    }
}

void show_temp(float temperature, float humidity) {
    unsigned long now = millis();
    if (now - last_display_time >= DISPLAY_INTERVAL_MS) {
        Serial.printf("Temperature %.2f C Humidity %.2f %%\n",
            temperature,
            humidity
        );
        char buffer[16];
        sprintf(buffer, "T:%.1fC H:%.1f%%", temperature, humidity);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(buffer);
        last_display_time = now;
    }
}

void read_sensor() {
    unsigned long now = millis();
    if (now - last_sensor_time >= SENSOR_INTERVAL_MS) {
        sensors_event_t humidity;
        sensors_event_t temperature;

        aht.getEvent(&humidity, &temperature);
        show_temp(temperature.temperature, humidity.relative_humidity);
        last_sensor_time = now;
    }
}

void loop() {
    if (! aht_ready) {
        delay(1000);
        return;
    }
    heartbeat();
    read_sensor();
}
