#include "Buttons.hpp"

#include "Logic.hpp"
#include "Pinout.hpp"
#include <driver/gpio.h>

Buttons::Buttons()
    : m_config {
        .pin_bit_mask = 0,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE,
    } {
    for (const auto i : Pins::Buttons)
        m_config.pin_bit_mask |= (1ULL << i);
}

bool IRAM_ATTR Buttons::read(gpio_num_t gpio) {
    constexpr gpio_dev_t *hw = &GPIO;
    if (gpio < 32) {
        return (hw->in >> gpio) & 0x1;
    } else {
        return (hw->in1.data >> (gpio - 32)) & 0x1;
    }
}

bool IRAM_ATTR Buttons::read(ButtonID id) {
    return read(Pins::Buttons[id]);
}

std::bitset<MaxID> IRAM_ATTR Buttons::readAll() {
    std::bitset<MaxID> out;
    for (int i = 0; i < MaxID; i++)
        out[i] = read(static_cast<gpio_num_t>(i));
    return out;
}

void Buttons::init() {
    gpio_config(&m_config);

    gpio_isr_register(trampoline, nullptr, 0, nullptr);
}

void IRAM_ATTR Buttons::trampoline(void* param) {
    Logic::singleton().m_callbackListButtons(readAll());
}
