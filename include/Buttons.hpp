#pragma once

#include "Error.hpp"
#include "Platform.hpp"
#include <array>
#include <bitset>
#include <driver/gpio.h>
#include <eventpp/callbacklist.h>
#include <eventpp/utilities/conditionalremover.h>
#include <eventpp/utilities/counterremover.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <soc/gpio_periph.h>

extern QueueHandle_t eventQueue;

class Buttons {
    friend class Logic;

public:
    using CallbackList = eventpp::CallbackList<void(std::bitset<MaxID> currentState, std::bitset<MaxID> changedBtns)>;

private:
    static constexpr const char* const m_tag = "Buttons";

    CallbackList m_callbackListButtons;
    QueueHandle_t m_eventQueue;

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

    CallbackList& callbackList() { return m_callbackListButtons; }

    static bool IRAM_ATTR precti(gpio_num_t cisloPinu) {
        return read(cisloPinu);
    }
    static bool IRAM_ATTR precti(ButtonID idTlacitka) {
        return read(idTlacitka);
    }
    static std::bitset<MaxID> IRAM_ATTR prectiVsechny() {
        return readAll();
    }

    /*
    buttons.onChange([](std::bitset<MaxID> currentState, std::bitset<MaxID> buttonChange) {
        if (buttonChange[up] == 1) {

            std::cout << "Button Up has changed and is now ";

            if (currentState[Up] == 1) {
                std::cout << "pressed." << std::endl;
            } else {
                std::cout << "released." << std::endl;
            }
        }
    });

    buttons.onChange([](std::bitset<MaxID> currentState) {
        std::cout << "Button Up is now ";

        if (currentState[Up] == 1) {
            std::cout << "pressed." << std::endl;
        } else {
            std::cout << "released." << std::endl;
        }
    });

    buttons.onChange([]() {
            std::cout << "A button has changed. No more information available" << std::endl;
        });

    buttons.onChange([](std::bitset<MaxID> currentState, std::bitset<MaxID> buttonChange) {
        std::cout << "You will see this only when Button Down changes, and the button is now ";

        if (currentState[Down] == 1) {
            std::cout << "pressed." << std::endl;
        } else {
            std::cout << "released." << std::endl;
        }
    }, Down);

    buttons.onChange([](bool isPressed) {
        std::cout << "You will see this only when Button Down changes" << std::endl;

        if (isPressed == 1) {
            std::cout << "pressed." << std::endl;
        } else {
            std::cout << "released." << std::endl;
        }
    }, Down);


    buttons.onChange([]() {
        std::cout << "You will see this only when Button Down changes" << std::endl;
    }, Down);
    */

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
    Buttons::CallbackList::Handle onNextChanges(Buttons::CallbackList::Callback function, int count) {
        return eventpp::counterRemover(m_callbackListButtons).append(function, count);
    }
    Buttons::CallbackList::Handle onNextChange(Buttons::CallbackList::Callback function) {
        return onNextChanges(function, 1);
    }
    CallbackList::Handle onNextChanges(std::function<void(bool isPressed)> function, ButtonID watchedButton, int count) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onNextChanges([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changedBtns) {
            if (changedBtns[watchedButton]) {
                function(currentState[watchedButton]);
            }
        },
            count);
    }
    CallbackList::Handle onNextChange(std::function<void(bool isPressed)> function, ButtonID watchedButton) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onNextChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changedBtns) {
            if (changedBtns[watchedButton]) {
                function(currentState[watchedButton]);
            }
        });
    }
    template<typename Condition>
    Buttons::CallbackList::Handle onChangeUntil(Buttons::CallbackList::Callback function, Condition condition) {
        return eventpp::conditionalRemover<Condition>(m_callbackListButtons).append(function, condition);
    }

    /*
        buttons.onPress([](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            std::cout << "A button has been pressed. ";

            if (changed[Right] == 1) {
                std::cout << "It was the Right button" << std::endl;
            } else {
                std::cout << "It was not the Right button" << std::endl;
            }
        });

        buttons.onPress([](std::bitset<MaxID> currentState) {
            std::cout << "A button has been pressed. ";

            std::cout << "Buttons ";

            for(int i = 0; i < MaxID; i++) {
                if (currentState[i] == 1) {
                    std::cout << i << ", ";
                }
            }

            std::cout << "are now pressed." << std::endl;
        });

        buttons.onPress([]() {
            std::cout << "A button has been pressed. No more information available" << std::endl;
        });

        buttons.onPress([]() {
            std::cout << "You will see this only when Button Left get pressed down." << std::endl;
        }, Left);
    */

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
    CallbackList::Handle onNextPresses(std::function<void()> function, ButtonID watchedButton, int count) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onNextChanges([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if (currentState[watchedButton] && changed[watchedButton]) {
                function();
            }
        },
            count);
    }

    CallbackList::Handle onNextPress(std::function<void()> function, ButtonID watchedButton) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onNextChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if (currentState[watchedButton] && changed[watchedButton]) {
                function();
            }
        });
    }

    template<typename Condition>
    Buttons::CallbackList::Handle onPressUntil(std::function<void()> function, Condition condition, ButtonID watchedButton) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onChangeUntil<Condition>([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if (currentState[watchedButton] && changed[watchedButton]) {
                function();
            }
        }, condition);
    }
    /*
        buttons.onRelease([](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            std::cout << "A button has been released. ";

            if (changed[Right] == 0) {
                std::cout << "It was the Right button" << std::endl;
            } else {
                std::cout << "It was not the Right button" << std::endl;
            }
        });

        buttons.onRelease([](std::bitset<MaxID> currentState) {
            std::cout << "A button has been released. ";

            std::cout << "Buttons ";

            for(int i = 0; i < MaxID; i++) {
                if (currentState[i] == 0) {
                    std::cout << i << ", ";
                }
            }

            std::cout << "are now released." << std::endl;
        });

        buttons.onRelease([]() {
            std::cout << "A button has been released. No more information available" << std::endl;
        });

        buttons.onRelease([]() {
            std::cout << "You will see this only when Button Left gets released." << std::endl;
        }, Left);
    */

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
    CallbackList::Handle onNextReleases(std::function<void()> function, ButtonID watchedButton, int count) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onNextChanges([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if (!currentState[watchedButton] && changed[watchedButton]) {
                function();
            }
        },
            count);
    }

    CallbackList::Handle onNexRelease(std::function<void()> function, ButtonID watchedButton) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onNextChange([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if (!currentState[watchedButton] && changed[watchedButton]) {
                function();
            }
        });
    }

    template<typename Condition>
    Buttons::CallbackList::Handle onReleaseUntil(std::function<void()> function, Condition condition, ButtonID watchedButton) {
        checkRange(watchedButton, 0, ButtonID::MaxID - 1, m_tag);
        return onChangeUntil<Condition>([=](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
            if (!currentState[watchedButton] && changed[watchedButton]) {
                function();
            }
        }, condition);
    }
    /*
        buttons.priZmene([](std::bitset<MaxID> aktualniStav, std::bitset<MaxID> zmenenaTlacitka) {
            if (zmenenaTlacitka[Up] == 1) {

                std::cout << "Tlačítko Nahoru se změnilo a teď je ";

                if (aktualniStav[Up] == 1) {
                    std::cout << "zmáčknuté." << std::endl;
                } else {
                    std::cout << "puštěné." << std::endl;
                }
            }
        });

        buttons.priZmene([](std::bitset<MaxID> aktualniStav) {
            std::cout << "Tlačítko Nahoru je nyní ";

            if (aktualniStav[Up] == 1) {
                std::cout << "zmáčknuté." << std::endl;
            } else {
                std::cout << "puštěné." << std::endl;
            }
        });

        buttons.priZmene([]() {
            std::cout << "Tlačítko bylo změněno. Není k dispozici více informací." << std::endl;
        });

        buttons.priZmene([](std::bitset<MaxID> aktualniStav, std::bitset<MaxID> zmenenaTlacitka) {
            std::cout << "Tohle uvidíte pouze pokud se změní tlačítko Dolů, a toto tlačítko je nyní ";

            if (aktualniStav[Down] == 1) {
                std::cout << "zmáčknuté." << std::endl;
            } else {
                std::cout << "puštěné." << std::endl;
            }
        }, Down);

        buttons.priZmene([](bool isPressed) {
            std::cout << "Tohle uvidíte pouze pokud se změní tlačítko Dolů" << std::endl;

            if (isPressed == 1) {
                std::cout << "zmáčknuté." << std::endl;
            } else {
                std::cout << "puštěné." << std::endl;
            }
        }, Down);


        buttons.onChange([]() {
            std::cout << "Tohle uvidíte pouze pokud se změní tlačítko Dolů" << std::endl;
        }, Down);
    */
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
    /*
        buttons.priStisku([](std::bitset<MaxID> aktualniStav, std::bitset<MaxID> zmena) {
            std::cout << "Nějaké tlačítko bylo zmáčknuto. ";

            if (zmena[Right] == 1) {
                std::cout << "Bylo to tlačítko Vpravo" << std::endl;
            } else {
                std::cout << "Nebylo to tlačítko Vpravo" << std::endl;
            }
        });

        buttons.priStisku([](std::bitset<MaxID> aktualniStav) {
            std::cout << "Nějaké tlačítko bylo zmáčknuto. ";

            std::cout << "Tlačítka ";

            for(int i = 0; i < MaxID; i++) {
                if (aktualniStav[i] == 1) {
                    std::cout << i << ", ";
                }
            }

            std::cout << "jsou nyní zmáčknutá." << std::endl;
        });

        buttons.priStisku([]() {
            std::cout << "Tlačítko bylo Zmáčknuto. Není k dispozici více informací." << std::endl;
        });

        buttons.priStisku([]() {
            std::cout << "Tohle uvidíte pouze pokud zmáčknete tlačítko Vlevo." << std::endl;
        }, Left);
    */
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
    /*
        buttons.priUvolneni([](std::bitset<MaxID> aktualniStav, std::bitset<MaxID> zmena) {
            std::cout << "Tlačítko bylo puštěno. ";

            if (zmena[Right] == 1) {
                std::cout << "Bylo to tlačítko Vpravo" << std::endl;
            } else {
                std::cout << "Nebylo to tlačítko Vpravo" << std::endl;
            }
        });

        buttons.priUvolneni([](std::bitset<MaxID> aktualniStav) {
            std::cout << "Tlačítko bylo puštěno. ";

            std::cout << "Tlačítka ";

            for(int i = 0; i < MaxID; i++) {
                if (aktualniStav[i] == 0) {
                    std::cout << i << ", ";
                }
            }

            std::cout << "jsou nyní puštěná." << std::endl;
        });

        buttons.priUvolneni([]() {
            std::cout << "Tlačítko bylo uvolněno. Není k dispozici více informací." << std::endl;
        });

        buttons.priUvolneni([]() {
            std::cout << "Tohle uvidíte pouze pokud pustíte tlačítko Vlevo." << std::endl;
        }, Left);
    */
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
