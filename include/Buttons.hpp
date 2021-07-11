#pragma once

#include "Error.hpp"
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
    friend class Logic;
    using CallbackList = eventpp::CallbackList<void(std::bitset<MaxID> currentState, std::bitset<MaxID> changedBtns)>;

private:
    static constexpr const char* const m_tag = "Buttons";

    CallbackList m_callbackListButtons;
    xQueueHandle m_eventQueue;

    static void callbacksTask(void* selfVoid);

    Buttons();
    void init();

    static void IRAM_ATTR isrHandler(void* selfVoid);

public:
    Buttons(Buttons&) = delete;
    Buttons& operator=(Buttons&) = delete;

    static bool IRAM_ATTR read(gpio_num_t);
    static bool IRAM_ATTR read(ButtonID);
    static std::bitset<MaxID> IRAM_ATTR readAll();

    static bool IRAM_ATTR precti(gpio_num_t cisloPinu) {
        return read(cisloPinu);
    }
    static bool IRAM_ATTR precti(ButtonID idTlacitka) {
        return read(idTlacitka);
    }
    static std::bitset<MaxID> IRAM_ATTR prectiVsechny() {
        return readAll();
    }

    CallbackList::Handle onChange(CallbackList::Callback function);
    CallbackList::Handle onChange(std::function<void(std::bitset<MaxID> currentState)> function) {
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID>) { function(currentState); });
    }
    CallbackList::Handle onChange(std::function<void()> function) {
        return onChange([=](std::bitset<MaxID>, std::bitset<MaxID>) { function(); });
    }

    CallbackList::Handle onChange(CallbackList::Callback function, ButtonID watchedButton) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changedBtns) {
            if (changedBtns[watchedButton]) {
                function(currentState, changedBtns);
            }
        });
    }
    CallbackList::Handle onChange(std::function<void(bool isPressed)> function, ButtonID watchedButton) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changedBtns) {
            if (changedBtns[watchedButton]) {
                function(currentState[watchedButton]);
            }
        });
    }
    CallbackList::Handle onChange(std::function<void()> function, ButtonID watchedButton) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changedBtns) {
            if (changedBtns[watchedButton]) {
                function();
            }
        });
    }

    CallbackList::Handle onPress(CallbackList::Callback function) {
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if ((changed & currentState).any()) {
                function(currentState, changed);
            }
        });
    }
    CallbackList::Handle onPress(std::function<void(std::bitset<MaxID> currentState)> function) {
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if ((changed & currentState).any()) {
                function(currentState);
            }
        });
    }
    CallbackList::Handle onPress(std::function<void()> function) {
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if ((changed & currentState).any()) {
                function();
            }
        });
    }

    CallbackList::Handle onPress(std::function<void()> function, ButtonID watchedButton) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if (currentState[watchedButton] && changed[watchedButton]) {
                function();
            }
        });
    }

    CallbackList::Handle onRelease(CallbackList::Callback function) {
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if ((changed & ~currentState).any()) {
                function(currentState, changed);
            }
        });
    }
    CallbackList::Handle onRelease(std::function<void(std::bitset<MaxID> currentState)> function) {
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if ((changed & ~currentState).any()) {
                function(currentState);
            }
        });
    }
    CallbackList::Handle onRelease(std::function<void()> function) {
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if ((changed & ~currentState).any()) {
                function();
            }
        });
    }

    CallbackList::Handle onRelease(std::function<void()> function, ButtonID watchedButton) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if (!currentState[watchedButton] && changed[watchedButton]) {
                function();
            }
        });
    }

    CallbackList::Handle priZmene(CallbackList::Callback funkce) { return onChange(funkce); }
    CallbackList::Handle priZmene(std::function<void(std::bitset<MaxID> aktualniStav)> funkce) { return onChange(funkce); }
    CallbackList::Handle priZmene(std::function<void()> funkce) { return onChange(funkce); }

    CallbackList::Handle priZmene(CallbackList::Callback funkce, ButtonID sledovaneTlacitko) { return onChange(funkce, sledovaneTlacitko); }
    CallbackList::Handle priZmene(std::function<void(bool jeStisknuto)> funkce, ButtonID sledovaneTlacitko) {
        return onChange(funkce, sledovaneTlacitko);
    }
    CallbackList::Handle priZmene(std::function<void()> funkce, ButtonID sledovaneTlacitko) {
        return onChange(funkce, sledovaneTlacitko);
    }

    CallbackList::Handle priStisku(CallbackList::Callback funkce) {
        return onPress(funkce);
    }
    CallbackList::Handle priStisku(std::function<void(std::bitset<MaxID> currentState)> funkce) {
        return onPress(funkce);
    }
    CallbackList::Handle priStisku(std::function<void()> funkce) {
        return onPress(funkce);
    }

    CallbackList::Handle priStisku(std::function<void()> funkce, ButtonID sledovaneTlacitko) {
        return onPress(funkce, sledovaneTlacitko);
    }

    CallbackList::Handle priUvolneni(CallbackList::Callback funkce) {
        return onRelease(funkce);
    }
    CallbackList::Handle priUvolneni(std::function<void(std::bitset<MaxID> currentState)> funkce) {
        return onRelease(funkce);
    }
    CallbackList::Handle priUvolneni(std::function<void()> funkce) {
        return onRelease(funkce);
    }

    CallbackList::Handle priUvolneni(std::function<void()> funkce, ButtonID sledovaneTlacitko) {
        return onRelease(funkce, sledovaneTlacitko);
    }

    void removeCallback(CallbackList::Handle);
    void smazatCallback(CallbackList::Handle handle) {
        removeCallback(handle);
    }
};
