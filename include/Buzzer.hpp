#pragma once

#include "Pinout.hpp"
#include <driver/gpio.h>
#include "driver/ledc.h"


class Buzzer {
    friend class Logic;

private:
    Buzzer()
        : m_on(false) {
        if constexpr(Pins::GeneratorBuzzer) {
            gpio_set_direction(Pins::Buzzer, GPIO_MODE_OUTPUT);
            gpio_set_level(Pins::Buzzer, 0);
        } else {
            ledc_timer_config_t ledc_timer = {
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .duty_resolution = LEDC_TIMER_10_BIT,
                .timer_num = LEDC_TIMER_0,
                .freq_hz = 2000,
                .clk_cfg = LEDC_AUTO_CLK
            };
            ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

            ledc_channel_config_t ledc_channel = {
                .gpio_num = Pins::Buzzer,
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .channel = LEDC_CHANNEL_0,
                .intr_type = LEDC_INTR_DISABLE,
                .timer_sel = LEDC_TIMER_0,
                .duty = 0, // 0% duty cycle
                .hpoint = 0,
                // .flags = { 0 }
            };
            ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
        }

    }

    bool m_on;

public:
    Buzzer(Buzzer&) = delete;
    Buzzer& operator=(Buzzer&) = delete;

    bool isOn() const {
        return m_on;
    }

    void on() {
        if(Pins::GeneratorBuzzer) {
            gpio_set_level(Pins::Buzzer, 1);
        } else {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 512);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        }
        m_on = true;
    }

    void off() {
        if(Pins::GeneratorBuzzer) {
            gpio_set_level(Pins::Buzzer, 0);
        } else {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        }
        m_on = false;
    }

    bool jeZapnuty() const {
        return m_on;
    }

    void zapnout() {
        on();
    }

    void vypnout() {
        off();
    }
};
