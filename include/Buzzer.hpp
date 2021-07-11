#pragma once

#include "Pinout.hpp"
#include <driver/gpio.h>

class Buzzer {
    friend class Logic;

private:
    Buzzer()
        : m_on(false) {
        gpio_set_direction(Pins::Buzzer, GPIO_MODE_OUTPUT);
        gpio_set_level(Pins::Buzzer, 0);
    }

    bool m_on;

public:
    Buzzer(Buzzer&) = delete;
    Buzzer& operator=(Buzzer&) = delete;

    bool isOn() const {
        return m_on;
    }

    void on() {
        gpio_set_level(Pins::Buzzer, 1);
        m_on = true;
    }

    void off() {
        gpio_set_level(Pins::Buzzer, 0);
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
