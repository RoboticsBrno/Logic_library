#pragma once

#include "Pinout.hpp"
#include <array>
#include <bitset>
#include <driver/gpio.h>
#include <eventpp/callbacklist.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <soc/gpio_periph.h>

extern xQueueHandle eventQueue;

class Buttons {
    using CallbackList = eventpp::CallbackList<void(std::bitset<MaxID> currentState, std::bitset<MaxID> changedBtns)>;

private:
    const char* m_tag = "Buttons";

    CallbackList m_callbackListButtons;
    xQueueHandle m_eventQueue;

    static void callbacksTask(void* selfVoid);

public:
    static bool IRAM_ATTR read(gpio_num_t);
    static bool IRAM_ATTR read(ButtonID);
    static std::bitset<MaxID> IRAM_ATTR readAll();

    static void IRAM_ATTR isrHandler(void* selfVoid);

    Buttons();

    void init();

    CallbackList::Handle onChange(CallbackList::Callback function);
    CallbackList::Handle onChange(std::function<void(std::bitset<MaxID> currentState)> function) {
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID>) { function(currentState); });
    }
    CallbackList::Handle onChange(std::function<void()> function) {
        return onChange([=](std::bitset<MaxID>, std::bitset<MaxID>) { function(); });
    }

    CallbackList::Handle onChange(CallbackList::Callback function, ButtonID watchedButton) {
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changedBtns) {
            if (changedBtns[watchedButton]) {
                function(currentState, changedBtns);
            }
        });
    }
    CallbackList::Handle onChange(std::function<void(bool isPressed)> function, ButtonID watchedButton) {
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changedBtns) {
            if (changedBtns[watchedButton]) {
                function(currentState[watchedButton]);
            }
        });
    }
    CallbackList::Handle onChange(std::function<void()> function, ButtonID watchedButton) {
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changedBtns) {
            if (changedBtns[watchedButton]) {
                function();
            }
        });
    }

    void removeCallback(CallbackList::Handle);
};
