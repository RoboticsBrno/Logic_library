#pragma once

#include "Display.hpp"
#include "Pinout.hpp"
#include "StatusBar.hpp"
#include <bitset>
#include <eventpp/callbacklist.h>
#include <functional>

class Logic {
    friend class Buttons;

private:
    using CallbackList = eventpp::CallbackList<void(std::bitset<MaxID>)>;

    const char* m_tag = "Logic";

    Display m_display;

    StatusBar m_statusBar;

    CallbackList m_callbackListButtons;

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

    CallbackList::Handle onButtonChange(CallbackList::Callback function);
    CallbackList::Handle onButtonChange(CallbackList::Callback function, CallbackList::Callback condition);

    void removeOnButtonChange(CallbackList::Handle);
};

void logicMain();

static Logic& logic = Logic::singleton();
static Display& display = logic.display();
static StatusBar& statusBar = logic.statusBar();