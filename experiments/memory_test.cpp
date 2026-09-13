#include <Arduino.h>

void show_memory();
void show_chip_info();
void show_miscellaneous();
void test_psram();
void show_free_psram();

constexpr size_t BYTES_PER_KIB = 1024;
constexpr size_t BYTES_PER_MIB = 1024.0 * 1024.0;

void setup() {
    Serial.begin(115200);
    delay(5000);
    Serial.println("ESP32-S3 says Hi");
}

void show_chip_info() {
    Serial.println();
    Serial.println("=== Chip info ===");
    Serial.printf("Flash model: %s\n", ESP.getChipModel());
    Serial.printf("Chip revision: %d\n", ESP.getChipRevision());
    Serial.printf("CPU cores: %d\n", ESP.getChipCores());
    Serial.printf("CPU frequency: %d MHz\n", ESP.getCpuFreqMHz());
}

void show_memory() {
    Serial.println();
    Serial.println("=== Flash ===");
    size_t flash_size = ESP.getFlashChipSize();
    Serial.printf("Flash size: %u bytes, %.4f MiB\n", flash_size, flash_size / (1024.0 * 1024.0));

    Serial.println();
    Serial.println("=== RAM ===");
    Serial.printf("Heap size: %d KiB\n", ESP.getHeapSize() / 1024);
    Serial.printf("Free heap: %d KiB\n", ESP.getFreeHeap() / 1024);

    Serial.println();
    Serial.println("=== PSRAM ===");
    size_t psram = ESP.getPsramSize();
    size_t free_psram = ESP.getFreePsram();
    Serial.printf("PSRAM size: %u bytes, %.4f MiB\n", psram, psram / (1024.0 * 1024.0));
    Serial.printf("Free PSRAM: %u bytes, %.4f MiB\n", free_psram, free_psram / (1024.0 * 1024.0));
}

void show_miscellaneous() {
    Serial.println();
    Serial.println("=== Miscellaneous ===");
    Serial.printf("SDK version: %s\n", ESP.getSdkVersion());
}

void show_free_psram() {
    size_t free_psram = ESP.getFreePsram();
    Serial.printf("PSRAM size: %u bytes, %.4f MiB\n", free_psram, free_psram / (1024.0 * 1024.0));
}

void test_psram() {
    constexpr size_t BUFFER_SIZE = 1024 * 1024;

    Serial.println();
    Serial.println("=== PSRAM allocation test ===");
    Serial.println("Before allocation");
    show_free_psram();

    u_int8_t *buffer = (u_int8_t *) ps_malloc(BUFFER_SIZE);
    if (buffer == nullptr) {
        Serial.println("Allocation failed");
        return;
    }

    Serial.printf("Allocated %u bytes\n", BUFFER_SIZE);
    show_free_psram();

    free(buffer);

    Serial.printf("Freed buffer\n");
    show_free_psram();
}

void show_info() {
    show_chip_info();
    show_memory();
    show_miscellaneous();
}

void loop() {
    show_info();
    test_psram();
    delay(5000);
}
