#pragma once

#include "Platform.hpp"
#include "driver/ledc.h"
#include <driver/gpio.h>

#include <cstdint>
#include <mutex>

class Buzzer {
    friend class Logic;

private:
    Buzzer();

    bool m_on = false;
    std::uint32_t m_frequency = 440;

    std::mutex m_mutex;

public:
    Buzzer(Buzzer&) = delete;
    Buzzer& operator=(Buzzer&) = delete;

    /**
     * @brief Set frequency of buzzer
     * 
     * @param frequency 
     */
    void setFrequency(std::uint32_t frequency);

    /**
     * @brief Get state of buzzer
     * 
     * @return true 
     * @return false 
     */
    bool isOn() const;

    /**
     * @brief Turn buzzer on
     * 
     */
    void on();

    /**
     * @brief Turn buzzer on with specified frequency
     * 
     * @param frequency 
     */
    void on(std::uint32_t frequency);

    /**
     * @brief Turn buzzer off
     * 
     */
    void off();

    /**
     * @brief Nastaví frekvenci bzučáku
     * 
     */
    void nastavitFrekvenci(std::uint32_t frekvence);

    /**
     * @brief Vrátí aktuální stav bzučáku
     * 
     * @return true 
     * @return false 
     */
    bool jeZapnuty() const;

    /**
     * @brief Zapne bzučák
     * 
     */
    void zapnout();

    /**
     * @brief Zapne bzučák s nastavenou frekvencí
     * 
     * @param frekvence 
     */
    void zapnout(std::uint32_t frekvence);

    /**
     * @brief Vypne bzučák
     * 
     */
    void vypnout();
};
