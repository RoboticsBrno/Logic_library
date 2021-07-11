#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdint.h>

/**
 * @brief Wait for specified amount of milliseconds
 *
 * @param ms number of milliseconds to wait
 */
inline void delay(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

/**
 * @brief Wait for specified amount of seconds
 *
 * @param seconds number of seconds to wait
 */
inline void waitSeconds(float seconds) {
    delay(uint32_t(seconds * 1000.f));
}

/**
 * @brief Čeká po specifikovaný počet vteřin
 *
 * @param vteriny Kolik vteřin se má čekat
 */
inline void cekejVteriny(float vteriny) {
    waitSeconds(vteriny);
}

/**
 * @brief Returns time since boot in milliseconds
 *
 * @return uint32_t number of milliseconds since boot
 */
inline uint32_t millis() {
    return pdTICKS_TO_MS(xTaskGetTickCount());
}

/**
 * @brief Returns time since boot in seconds
 *
 * @return float number of seconds since boot
 */
inline float seconds() {
    return float(millis()) / 1000.f;
}

/**
 * @brief Vrátí počet vteřin od začátku programu
 *
 * @return float počet vteřin od začátku programu
 */
inline float vteriny() {
    return seconds();
}

inline long random(long howbig) {
    uint32_t x = esp_random();
    uint64_t m = uint64_t(x) * uint64_t(howbig);
    uint32_t l = uint32_t(m);
    if (l < howbig) {
        uint32_t t = -howbig;
        if (t >= howbig) {
            t -= howbig;
            if (t >= howbig)
                t %= howbig;
        }
        while (l < t) {
            x = esp_random();
            m = uint64_t(x) * uint64_t(howbig);
            l = uint32_t(m);
        }
    }
    return m >> 32;
}

inline long random(long howsmall, long howbig) {
    if (howsmall >= howbig) {
        return howsmall;
    }
    long diff = howbig - howsmall;
    return random(diff) + howsmall;
}

inline long nahodne(long maximum) {
    return random(maximum);
}

inline long nahodne(long minimum, long maximum) {
    return random(minimum, maximum);
}
