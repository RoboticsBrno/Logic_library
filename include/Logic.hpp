#pragma once

#include "Buttons.hpp"
#include "Display.hpp"
#include "Pinout.hpp"
#include "StatusBar.hpp"
#include "TimeUtils.hpp"
#include <bitset>
#include <eventpp/callbacklist.h>
#include <functional>

class Logic {
    friend class Buttons;
    friend void trampoline(void*);

private:
    const char* m_tag = "Logic";

    Display m_display;

    StatusBar m_statusBar;

    Buttons m_buttons;

    Logic();

public:
    Logic(Logic&) = delete;
    Logic(Logic&&) = delete;
    Logic& operator=(Logic&) = delete;
    Logic& operator=(Logic&&) = delete;

    static Logic& IRAM_ATTR singleton();

    void init();
    void turnOnLeds();
    void turnOffLeds();

    ~Logic() = default;

    Display& display();

    StatusBar& statusBar();

    Buttons& buttons();
};

void logicMain();

extern Logic& logic;
extern Display& display;
extern StatusBar& statusBar;
extern Buttons& buttons;
