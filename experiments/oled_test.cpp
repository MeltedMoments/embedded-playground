#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>
#include <Adafruit_AHTX0.h>
#include <algorithm>
#include  <Adafruit_GFX.h>
#include  <Adafruit_SH110X.h>


// Hardware 
constexpr int OLED_MOSI = 11;       // SPI MOSI/data
constexpr int OLED_CLK  = 12;       // SPI clock
constexpr int OLED_CS   = 10;       // chip select
constexpr int OLED_DC   = 9;        // data/command
constexpr int OLED_RST  = 8;        // reset

constexpr int OLED_WIDTH = 64;  
constexpr int OLED_HEIGHT  = 128; 

// constexpr int SDA_PIN = 4;
// constexpr int SCL_PIN = 6;
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

unsigned long last_heartbeat_time = 0;
unsigned long last_sensor_time = 0;
unsigned long last_display_time = 0;
float current_temperature;
float current_humidity;
bool aht_ready = false;
bool heartbeat_on = false;
bool have_reading = false;
Adafruit_SH1107 display(
    OLED_WIDTH,
    OLED_HEIGHT,
    OLED_MOSI,
    OLED_CLK,
    OLED_DC,
    OLED_RST,
    OLED_CS
);

// Adafruit_AHTX0 aht;
// rgb_lcd lcd; 

void setup() {
    Serial.begin(115200);
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

//     // delay(1000);
//     // Set up the AHT20
//     Wire.begin(SDA_PIN, SCL_PIN);
//     Wire.setTimeOut(100);
//     aht_ready = aht.begin(&Wire);
//     if (! aht_ready) {
//         Serial.println("AHT20 init failed");
//         return;
//     }
//     Serial.println("AHT20 ready");

//     // Set up the LCD display
//     lcd.begin(LCD_COLS, LCD_ROWS);
//     lcd.setCursor(0, 0);
//     lcd.print("Hello ESP");
//     lcd.setCursor(0, 1);
//     lcd.print("Ready");
//     // Show display 1 sec after startup
//     last_display_time = millis() - (DISPLAY_INTERVAL_MS - START_DISPLAY_DELAY_MS);
// }

// void show_heartbeat() {
//     heartbeat_on = ! heartbeat_on;
//     lcd.setCursor(15, 1);
//     lcd.print(heartbeat_on ? '.' : ' ');
// }

void heartbeat() {
    unsigned long now = millis();
    if (now - last_heartbeat_time < HEARTBEAT_INTERVAL_MS) {
        return;
    }

    Serial.println("Heartbeat");
    // show_heartbeat();
    last_heartbeat_time = now;
}

// V.Good: 100, Good: 70-99, Okay: 40-69, Bad: 10-39, V.Bad: 0-9
// String comfort_text (float comfort) {
//     if (comfort >= 100.0) return "V.Good";
//     if (comfort >= 70.0)  return "Good  ";
//     if (comfort >= 40.0)  return "Okay  ";
//     if (comfort >= 10.0)  return "Bad   ";
//     return "V.Bad ";
// }

// float calculate_comfort_score(float temperature, float humidity) {
//     float score = 100.0;     // best possible
//     // Penalise the score for every PENALTY away from the ideal, 
//     // for both temp and humidity
//     score -= abs(temperature - IDEAL_TEMPERATURE) * TEMPERATURE_PENALTY;
//     score -= abs(humidity - IDEAL_HUMIDITY) * HUMIDITY_PENALTY;

//     score = std::max(0.0F, score);
//     score = std::min(100.0F, score);
//     return score;
// }

// void show_temp(float temperature, float humidity) {
//     float comfort = calculate_comfort_score(temperature, humidity);
//     Serial.printf("Temperature %.2f C Humidity %.2f %%, Score: %.2f\n",
//         temperature,
//         humidity,
//         comfort
//     );
    
//     // Line 1
//     String buffer = "T:";
//     buffer += String(temperature, 1);
//     buffer += "C H:";
//     buffer += String(humidity, 1);
//     buffer += "%";
//     lcd.setCursor(0, 0);
//     lcd.print(buffer);

//     // Line 2 
//     buffer = "Comfort: ";
//     buffer += comfort_text(comfort);
//     lcd.setCursor(0,1);
//     lcd.print(buffer);
// }

// void update_display() {
//     unsigned long now = millis();
//     if (now - last_display_time < DISPLAY_INTERVAL_MS) {
//         return;
//     }

//     // if (now < next_display_time) {
//     //     return;
//     // }
//     if (! have_reading) {
//         Serial.println("No reading");
//         return;
//     }
//     show_temp(current_temperature, current_humidity);
//     last_display_time = now;
//         // next_display_time = now + 1000;

//     // next_display_time = now + DISPLAY_INTERVAL_MS;
// }

// void read_sensor() {
//     unsigned long now = millis();
//     if (now - last_sensor_time < SENSOR_INTERVAL_MS) {
//         return;
//     }

//     sensors_event_t humidity;
//     sensors_event_t temperature;
//     aht.getEvent(&humidity, &temperature);
//     current_temperature = temperature.temperature;
//     current_humidity = humidity.relative_humidity;
//     have_reading = true;
//     last_sensor_time = now;
// }

void loop() {
    // if (! aht_ready) {
    //     delay(1000);
    //     return;
    // }
    // heartbeat();
    // read_sensor();
    // update_display();
}
