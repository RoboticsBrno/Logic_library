#pragma once

#include "Pinout.hpp"
#include <SmartLeds.h>
#include <array>
#include <mutex>

enum States {
    A = 0,
    B,
    C,
    D,
    E,
};

using Stavy = States;

class StatusBar {
    friend class Logic;

private:
    const char* m_tag = "StatusBar";

    SmartLed m_statusLeds;

    mutable std::mutex m_mutex;

    static constexpr int s_count = 5;

    int m_state;
    Rgb m_color;

    StatusBar(Rgb color = Rgb(255, 255, 255), int count = 5, int i_channel = 1);

    std::array<Rgb, 5> m_frame;
public:
    StatusBar(StatusBar&) = delete;
    StatusBar(StatusBar&&) = delete;
    StatusBar& operator=(StatusBar&) = delete;
    StatusBar& operator=(StatusBar&&) = delete;

    ~StatusBar() = default;

    void setState(int state);
    void nastavStav(int stav) { setState(stav); }

    int state() const;
    int stav() const { return state(); }

    void setColor(Rgb color);
    void nastavBarvu(Rgb barva) { setColor(barva); }

    Rgb color() const;
    Rgb barva() const { return color(); }

    void show();
    void ukaz() { show(); }

    void show(int intensity);
    void ukaz(int intenzita) { show(intenzita); }

    void clear();
    void vycisti() { clear(); }

    Rgb& at(int x);
    Rgb& pozice(int x) { return at(x); }

    Rgb& operator[](int i_index);
};
using StavovyRadek = StatusBar;
