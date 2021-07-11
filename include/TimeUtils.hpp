#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdint.h>

inline void delay(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

inline void waitSeconds(float seconds) {
    delay(uint32_t(seconds * 1000.f));
}

inline void cekejVteriny(float vteriny) {
    waitSeconds(vteriny);
}

inline uint32_t millis() {
    return pdTICKS_TO_MS(xTaskGetTickCount());
}

inline float seconds() {
    return float(millis()) / 1000.f;
}

inline float vteriny() {
    return seconds();
}
