#pragma once

#include <array>
#include <esp_debug_helpers.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string>

#ifndef ENABLE_CZECH_ERRORS
#define ENABLE_CZECH_ERRORS false
#endif

[[gnu::always_inline]] inline void sanityCheck(bool expression, const char* tag, const char* errorMessage, unsigned stackTraceDepth = 10) {
    if (!expression) {
        ESP_LOGE(tag, "%s", errorMessage);
        esp_backtrace_print(stackTraceDepth);
        while (true)
            vTaskDelay(100000 / portTICK_PERIOD_MS);
    }
}

enum ErrorCodes {
    NoError = 0,
    VariableOverflow = 1,
    VariableUnderflow,
};

static const std::string errors[] = {
    "No error",
    "Variable overflow",
    "Variable underflow",
};

static const std::string errorsCs[] = {
    "Zadna chyba",
    "Preteceni promenne",
    "Podteceni promenne",
};

[[gnu::always_inline]] inline void sanityCheck(bool expression, const char* tag, ErrorCodes errorCode, unsigned stackTraceDepth = 10) {
    sanityCheck(expression, tag, (ENABLE_CZECH_ERRORS ? errorsCs : errors)[errorCode].c_str(), stackTraceDepth);
}

[[gnu::always_inline]] inline void checkRange(int value, int min, int max, const char* tag, unsigned stackTraceDepth = 10) {
    sanityCheck(value <= max, tag, VariableOverflow);
    sanityCheck(value >= min, tag, VariableUnderflow);
}
