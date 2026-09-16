#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

constexpr int PIXEL_PIN = 4;
constexpr int PIXEL_COUNT = 8;
constexpr int MIN_LEVEL = 0;
constexpr int MAX_LEVEL = 100;
constexpr int DIVISOR = (MAX_LEVEL - MIN_LEVEL) / PIXEL_COUNT;
constexpr float RISE_FACTOR = 0.85;
constexpr float FALL_FACTOR = 0.3;
constexpr float HYSTERESIS = 3.0;

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
unsigned int current_level = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("NeoPixel Strip test");

    strip.begin();
    strip.setBrightness(50);
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

void show_noise_level(float raw_intensity) {      
    if (raw_intensity < MIN_LEVEL) {
        raw_intensity = MIN_LEVEL;
    }
    if (raw_intensity > MAX_LEVEL) {
        raw_intensity = MAX_LEVEL;
    }

    int level = current_level;
    float intensity = raw_intensity - MIN_LEVEL;
    float lower_threshold = current_level * DIVISOR - HYSTERESIS;
    float upper_threshold = (current_level + 1) * DIVISOR + HYSTERESIS;
    float threshold = current_level * DIVISOR;   
    if (   (intensity < lower_threshold) 
        || (intensity > upper_threshold)) {
            level = intensity / DIVISOR;   
    }

    Serial.printf(
        "current: %d new: %d\r\n", 
        current_level,
        level
    );
    if (level < 0) {
        level = 0;
    } else if (level > PIXEL_COUNT) {
        level = PIXEL_COUNT;
    } 
    current_level = level;
    show_pixels(level);
}

// float readings[SMOOTHING_WINDOW_SIZE] = {0};
// int reading_count = 0; // To take care of the start of readings
float smoothed_intensity = 0;
float smooth_intensity(float intensity) {
    float factor; 
    if (intensity > smoothed_intensity) {
        factor = RISE_FACTOR;
    } else {
        factor = FALL_FACTOR;
    }

    // calculate exponential smoothing average;
    smoothed_intensity += factor * (intensity - smoothed_intensity);
    return smoothed_intensity;
}

const int data[] = {
    35, 38, 36, 42, 39,
    55, 58, 59, 60, 61, 
    62, 61, 60, 59, 61, 
    62, 64, 62, 60, 58,
    56, 48, 63, 51, 58,
    88, 73, 92, 84, 89,
    61, 57, 66, 54, 59,
    40, 37, 43, 35, 37
};int data_index = 0;
int data_len = sizeof(data) / sizeof(data[0]);
void read_sensor() {
    unsigned long now = millis();
    if (now - last_sensor_time >= SENSOR_INTERVAL_MS) {
        float raw = (float) data[data_index];
        // float smoothed = raw; //smooth_intensity(raw);
        float smoothed = smooth_intensity(raw);
        Serial.printf("raw: %.2f smoothed %.2f ", raw, smoothed);
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


// void show_noise_level(float raw_intensity) {      
//     if (raw_intensity < MIN_LEVEL) {
//         raw_intensity = MIN_LEVEL;
//     }
//     if (raw_intensity > MAX_LEVEL) {
//         raw_intensity = MAX_LEVEL;
//     }

//     int intensity = (raw_intensity - MIN_LEVEL) / DIVISOR;
//     int level = (intensity - MIN_LEVEL) / DIVISOR;
//     float threshold = level * DIVISOR;
//     if 
//     Serial.printf(
//         "intensity: %.2f level %d\r\n", 
//         intensity, 
//         level
//     );
//     show_pixels(level);
// }
//     // move older readings back
//     for (int i = SMOOTHING_WINDOW_SIZE - 1; i > 0; i--) {
//         readings[i] = readings[i-1];
//     }
//     // add newest reading
//     readings[0] = intensity;

//     // How many real readings do we have?
//     if (reading_count < SMOOTHING_WINDOW_SIZE) {
//         reading_count++;
//     }

//     float total = 0.0;
//     for (int i=0; i < reading_count; i++) {
//         total += readings[i];
//     }
//     return total / reading_count;
// }

// void print_window(const float *array, int size) {
//     if (size == 0 || array == NULL) {
//         Serial.print("[]");
//         return;
//     }

//     Serial.print(" [");
//     for (int i = 0; i < size; i++) {
//         Serial.print(array[i], 2);
//         if (i < size - 1) {
//             Serial.print(", ");
//         }
//     }
//     Serial.print("] ");
// }

// const int data[] = {60, 48, 54, 55, 85, 100, 55, 48, 61, 58, 55, 61, 55, 62, 55, 88, 20, 33, 20, 55};
// const int data[] = {40, 40, 40, 90, 90, 90, 90, 110, 100, 90,  40, 40, 40, 40, 40, 25, 25, 25, 25, 20, 20, 10, 10, 20, 30,};

