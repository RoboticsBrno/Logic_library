#include "Buzzer.hpp"

#include "Platform.hpp"
#include <driver/gpio.h>
#include <driver/ledc.h>

Buzzer::Buzzer() {
    std::lock_guard lock(m_mutex);
    if constexpr (!Platform::AdjustableBuzzer) {
        gpio_set_direction(Platform::Pins::Buzzer, GPIO_MODE_OUTPUT);
        gpio_set_level(Platform::Pins::Buzzer, 0);
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
            .gpio_num = Platform::Pins::Buzzer,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel = LEDC_CHANNEL_0,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LEDC_TIMER_0,
            .duty = 0, // 0% duty cycle
            .hpoint = 0,
            .flags = { 0 }
        };
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
        // ESP_ERROR_CHECK(ledc_fade_func_install(0));
    }
}

void Buzzer::setFrequency(std::uint32_t frequency) {
    if (!Platform::AdjustableBuzzer) {
        return;
    }

    std::lock_guard lock(m_mutex);

    ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, frequency);

    m_frequency = frequency;
}

bool Buzzer::isOn() const {
    return m_on;
}

void Buzzer::on() {
    std::lock_guard lock(m_mutex);
    if constexpr (!Platform::AdjustableBuzzer) {
        gpio_set_level(Platform::Pins::Buzzer, 1);
    } else {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 512);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    }
    m_on = true;
}

void Buzzer::on(std::uint32_t frequency) {
    if (Platform::AdjustableBuzzer) {
        setFrequency(frequency);
    }
    on();
}

void Buzzer::off() {
    std::lock_guard lock(m_mutex);
    if constexpr (!Platform::AdjustableBuzzer) {
        gpio_set_level(Platform::Pins::Buzzer, 0);
    } else {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    }
    m_on = false;
}

bool Buzzer::jeZapnuty() const {
    return m_on;
}

void Buzzer::zapnout() {
    on();
}

void Buzzer::zapnout(std::uint32_t frekvence) {
    on(frekvence);
}

void Buzzer::vypnout() {
    off();
}

void Buzzer::nastavitFrekvenci(std::uint32_t frekvence) {
    setFrequency(frekvence);
}
