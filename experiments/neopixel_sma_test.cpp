// Simple moving average to smooth intensity

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

constexpr int PIXEL_PIN = 4;
constexpr int PIXEL_COUNT = 8;
constexpr int MIN_LEVEL = 0;
constexpr int MAX_LEVEL = 100;
constexpr int DIVISOR = (MAX_LEVEL - MIN_LEVEL) / PIXEL_COUNT;
constexpr int SMOOTHING_WINDOW_SIZE = 4;   // number of readings in the moving average

constexpr unsigned long REPORT_INTERVAL_MS = 5000;
constexpr unsigned long SENSOR_INTERVAL_MS = 150;

Adafruit_NeoPixel strip(
    PIXEL_COUNT,
    PIXEL_PIN,
    NEO_GRB + NEO_KHZ800
);
const uint32_t GREEN  = strip.Color(0, 255, 0);
const uint32_t YELLOW = strip.Color(255, 180, 0);
const uint32_t RED    = strip.Color(255, 0, 0);

unsigned long last_report_time = 0;
unsigned long last_sensor_time = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("NeoPixel Strip test");

    strip.begin();
    strip.clear();
    strip.show();
}

void heartbeat() {
    unsigned long now = millis();
    if (now - last_report_time >= REPORT_INTERVAL_MS) {
        Serial.println("Heartbeat");
        last_report_time = now;
    }
}

const uint32_t colours[] = {
    GREEN, GREEN, GREEN, GREEN,
    YELLOW, YELLOW, RED, RED,    
};

// pass in number of leds to light
void show_pixels(int pixels) {
    strip.clear();
    for (int i=0; i < pixels; i++) {
        strip.setPixelColor(i, colours[i]);
    }
    strip.show();
}

void show_noise_level(float intensity) {      
    if (intensity < MIN_LEVEL) {
        intensity = MIN_LEVEL;
    }
    if (intensity > MAX_LEVEL) {
        intensity = MAX_LEVEL;
    }

    int level = (intensity - MIN_LEVEL) / DIVISOR;
    Serial.printf(
        "intensity: %.2f level %d\r\n", 
        intensity, 
        level
    );
    show_pixels(level);
}

float readings[SMOOTHING_WINDOW_SIZE] = {0};
int reading_count = 0; // To take care of the start of readings

float smooth_intensity(float intensity) {
    // move older readings back
    for (int i = SMOOTHING_WINDOW_SIZE - 1; i > 0; i--) {
        readings[i] = readings[i-1];
    }
    // add newest reading
    readings[0] = intensity;

    // How many real readings do we have?
    if (reading_count < SMOOTHING_WINDOW_SIZE) {
        reading_count++;
    }

    float total = 0.0;
    for (int i=0; i < reading_count; i++) {
        total += readings[i];
    }
    return total / reading_count;
}

void print_window(const float *array, int size) {
    if (size == 0 || array == NULL) {
        Serial.print("[]");
        return;
    }

    Serial.print(" [");
    for (int i = 0; i < size; i++) {
        Serial.print(array[i], 2);
        if (i < size - 1) {
            Serial.print(", ");
        }
    }
    Serial.print("] ");
}

const int data[] = {60, 48, 54, 55, 85, 100, 55, 48, 61, 58, 55, 61, 55, 62, 55, 88, 20, 33, 20, 55};
int data_index = 0;
int data_len = sizeof(data) / sizeof(data[0]);
void read_sensor() {
    unsigned long now = millis();
    if (now - last_sensor_time >= SENSOR_INTERVAL_MS) {
        float raw = (float) data[data_index];
        float smoothed = smooth_intensity(raw);
        Serial.printf("raw: %.2f smoothed %.2f ", raw, smoothed);
        print_window(readings, SMOOTHING_WINDOW_SIZE);
        show_noise_level(smoothed);
        data_index += 1;
        if (data_index >= data_len) {
            data_index = 0; 
        }
        last_sensor_time = now;
    }
}

void loop() {
    heartbeat();
    read_sensor();
}

// void lightshow() {
//     unsigned long now = millis();
//     if (now - last_sensor_time < SENSOR_INTERVAL_MS) {
//         return;
//     }

//     static int level = 0;
//     static int direction = 1;

//     strip.clear();
//     for (int i=0; i < level; i++) {
//         strip.setPixelColor(i, strip.Color(0, 255, 255));
//         delay(100);
//     }

//     strip.show();
//     level += direction;

//     if (level >= PIXEL_COUNT) {
//         level = PIXEL_COUNT;
//         direction = -1;
//     } else if (level <= 0) {
//         level = 0;
//         direction = 1;
//     }
// }
