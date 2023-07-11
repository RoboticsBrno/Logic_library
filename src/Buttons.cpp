#include "Buttons.hpp"

#include "Logic.hpp"
#include "Platform.hpp"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

Buttons::Buttons() {
}

bool IRAM_ATTR Buttons::read(gpio_num_t gpio) {
    constexpr gpio_dev_t* hw = &GPIO;
    if (gpio < 32) {
        return ((hw->in >> gpio) & 0x1) == 0;
    } else {
        return ((hw->in1.data >> (gpio - 32)) & 0x1) == 0;
    }
}

bool IRAM_ATTR Buttons::read(ButtonID id) {
    checkRange(id, 0, ButtonID::MaxID - 1, m_tag);
    return read(Platform::Pins::Buttons[id]);
}

std::bitset<MaxID> IRAM_ATTR Buttons::readAll() {
    std::bitset<MaxID> out;
    for (int i = 0; i < MaxID; i++)
        out[i] = read(ButtonID(i));
    return out;
}

void Buttons::init() {
    gpio_config_t config = {
        .pin_bit_mask = 0,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE,
    };

    for (const auto i : Platform::Pins::Buttons)
        config.pin_bit_mask |= (1ULL << i);

    gpio_config(&config);

    m_eventQueue = xQueueCreate(10, sizeof(std::bitset<MaxID>));
    xTaskCreate(callbacksTask, "logicBtnCb", 4096, this, 5, nullptr);

    gpio_install_isr_service(0);
    for (const auto i : Platform::Pins::Buttons)
        gpio_isr_handler_add(i, isrHandler, this);
}

void IRAM_ATTR Buttons::isrHandler(void* selfVoid) {
    auto* self = ((Buttons*)selfVoid);
    std::bitset<MaxID> currentState(readAll());
    xQueueSendFromISR(self->m_eventQueue, &currentState, nullptr);
}

void Buttons::callbacksTask(void* selfVoid) {
    auto* self = ((Buttons*)selfVoid);
    vTaskDelay(pdMS_TO_TICKS(150));
    auto lastState = readAll();
    std::bitset<MaxID> currentState;

    while (true) {
        if (!xQueueReceive(self->m_eventQueue, &currentState, portMAX_DELAY))
            continue;

        auto changed = lastState ^ currentState;
        lastState = currentState;
        if (changed.none())
            continue;

        self->m_callbackListButtons(currentState, changed);
    }
}

Buttons::CallbackList::Handle Buttons::onChange(Buttons::CallbackList::Callback function) {
    return m_callbackListButtons.append(function);
}

void Buttons::removeCallback(CallbackList::Handle handle) {
    m_callbackListButtons.remove(handle);
}
