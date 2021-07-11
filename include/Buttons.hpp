#pragma once

#include "Pinout.hpp"
#include <array>
#include <bitset>
#include <driver/gpio.h>
#include <soc/gpio_periph.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

extern xQueueHandle eventQueue;

class Buttons {
private:
    const char* m_tag = "Buttons";

    gpio_config_t m_config;

public:
    static bool IRAM_ATTR read(gpio_num_t);
    static bool IRAM_ATTR read(ButtonID);
    static std::bitset<MaxID> IRAM_ATTR readAll();

    static void IRAM_ATTR isrHandler(void*);

    Buttons();

    void init();
};
