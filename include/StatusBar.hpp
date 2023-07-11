#pragma once

#include "Platform.hpp"
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

    /**
     * @brief Set progress
     * 
     * @param state ID of state to set/progress made
     */
    void setState(int state);

    /**
     * @brief Nastaví stav
     * 
     * @param stav ID stavu/dokončený progress
     */
    void nastavStav(int stav) { setState(stav); }

    /**
     * @brief Returns current state
     * 
     * @return int Current state
     */
    int state() const;

    /**
     * @brief Vrátí aktuálně nastavený stav
     * 
     * @return int aktuální stav
     */
    int stav() const { return state(); }

    /**
     * @brief Set the color of status bar
     * 
     * @param color 
     */
    void setColor(Rgb color);

    /**
     * @brief Nastaví barvu stavové řádky
     * 
     * @param barva 
     */
    void nastavBarvu(Rgb barva) { setColor(barva); }

    /**
     * @brief Returns current color of status bar
     * 
     * @return Rgb current status bar color
     */
    Rgb color() const;

    /**
     * @brief Vrátí aktuální barvu stavového řádku
     * 
     * @return Rgb aktuální barva stavového řádku
     */
    Rgb barva() const { return color(); }

    /**
     * @brief Show current frame/state
     * 
     * @param intensity [0-255] maximal intenzity
     */
    void show(int intensity = 255);

    /**
     * @brief Vykreslí aktuální snímek/stav
     * 
     * @param intenzita [0-255] maximální intenzita na jejíž hodnotu se barvy uprav
     */
    void ukaz(int intenzita = 255) { show(intenzita); }

    /**
     * @brief Clear the display
     */
    void clear();

    /**
     * @brief Vyčistí celý displej
     */
    void vycisti() { clear(); }

    /**
     * @brief Return pixel at given position
     * 
     * @param x position
     * @return Rgb& Reference to pixel at position x
     */
    Rgb& at(int x);

    /**
     * @brief Vrátí pixel na dané pozici
     * 
     * @param x pozice
     * @return Rgb& Reference na požadovaný pixel
     */
    Rgb& pozice(int x) { return at(x); }

    Rgb& operator[](int i_index);
};
using StavovyRadek = StatusBar;
