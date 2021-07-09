#include "Display.hpp"

#include "Error.hpp"
#include "errno.h"
#include <driver/gpio.h>
#include <esp_log.h>
#include <mutex>

void Display::StatusBar::render() {
    m_parent.clearStatusBar();
    for (int i = 0; i < m_state; i++)
        m_parent.atStatusBar(i) = m_color;
}

Display::StatusBar::StatusBar(Display& parent, Rgb color)
    : m_parent(parent)
    , m_state(0)
    , m_color(color) {
}

void Display::StatusBar::setState(int state) {
    sanityCheck(state <= 5, m_tag, VariableOverflow);
    sanityCheck(state >= 0, m_tag, VariableUnderflow);
    m_state = state;
    render();
}

void Display::StatusBar::nastavStav(int stav) {
    sanityCheck(stav <= 5, m_tag, VariableOverflow);
    sanityCheck(stav >= 0, m_tag, VariableUnderflow);
    m_state = stav;
    render();
}

int Display::StatusBar::state() const {
    return m_state;
}

int Display::StatusBar::stav() const {
    return m_state;
}

Rgb Display::StatusBar::barva() const {
    return m_color;
}

Rgb Display::StatusBar::color() const {
    return m_color;
}

void Display::StatusBar::nastavBarvu(Rgb barva) {
    m_color = barva;
    render();
}

void Display::StatusBar::setColor(Rgb color) {
    m_color = color;
    render();
}

Display::StatusBar& Display::StatusBar::operator+=(int other) {
    sanityCheck(m_state < (6 - other), m_tag, VariableOverflow);
    m_state += other;
    render();
    return *this;
}

Display::StatusBar& Display::StatusBar::operator-=(int other) {
    sanityCheck(m_state > other, m_tag, VariableUnderflow);
    m_state -= other;
    render();
    return *this;
}

Display::StatusBar& Display::StatusBar::operator++() {
    sanityCheck(m_state < 5, m_tag, VariableOverflow);
    m_state++;
    render();
    return *this;
}

Display::StatusBar& Display::StatusBar::operator--() {
    sanityCheck(m_state > 0, m_tag, VariableUnderflow);
    m_state--;
    render();
    return *this;
}

int Display::StatusBar::operator++(int other) {
    sanityCheck(m_state < 5, m_tag, VariableOverflow);
    int out = m_state;
    m_state++;
    render();
    return out;
}

int Display::StatusBar::operator--(int other) {
    sanityCheck(m_state > 0, m_tag, VariableUnderflow);
    int out = m_state;
    m_state--;
    render();
    return out;
}

Display::StatusBar::operator int() const {
    return m_state;
}

Display::StatusBar& Display::StatusBar::operator=(int state) {
    sanityCheck(state <= 5, m_tag, VariableOverflow);
    sanityCheck(state >= 0, m_tag, VariableUnderflow);
    m_state = state;
    render();
    return *this;
}

void Display::displayBresenhmCircle(int i_xCenter, int i_yCenter, int i_x, int i_y, Rgb i_color) {
    at(i_xCenter + i_x, i_yCenter + i_y) = i_color;
    at(i_xCenter - i_x, i_yCenter + i_y) = i_color;
    at(i_xCenter + i_x, i_yCenter - i_y) = i_color;
    at(i_xCenter - i_x, i_yCenter - i_y) = i_color;
    at(i_xCenter + i_y, i_yCenter + i_x) = i_color;
    at(i_xCenter - i_y, i_yCenter + i_x) = i_color;
    at(i_xCenter + i_y, i_yCenter - i_x) = i_color;
    at(i_xCenter - i_y, i_yCenter - i_x) = i_color;
}

Display::Display(int i_width, int i_eight, int i_channelDisplay, int i_channelStatus)
    : m_displayLeds(LED_WS2812, i_width * i_eight, Pins::Display, i_channelDisplay)
    , m_statusLeds(LED_WS2812, 5, Pins::Status, i_channelStatus)
    , m_width(i_width)
    , m_height(i_eight)
    , m_statusBar(*this, Rgb(255, 255, 255)) {
}

Rgb& Display::at(int i_x, int i_y) {
    return m_displayLeds[(i_y * m_width) + i_x];
}

Rgb& Display::atStatusBar(int x) {
    return m_statusLeds[x];
}

void Display::init() {
    gpio_config_t configData = {
        .pin_bit_mask = 1ULL << Pins::LedPower,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&configData);
}

void Display::turnOn() {
    gpio_set_level(Pins::LedPower, 1);
}

void Display::turnOff() {
    gpio_set_level(Pins::LedPower, 0);
}

void Display::clear() {
    for (int i = 0; i < m_width * m_height; i++)
        m_displayLeds[i] = Rgb(0, 0, 0);
}

void Display::clearStatusBar() {
    for (int i = 0; i < 5; i++)
        m_statusLeds[i] = Rgb(0, 0, 0);
}

void Display::drawRectangle(int i_x, int i_y, int i_width, int i_height, Rgb i_color) {
    for (int y = i_y; y < i_y + i_height; y++)
        for (int x = i_x; x < i_x + i_width; x++)
            at(x, y) = i_color;
}

void Display::drawSquare(int i_x, int i_y, int i_size, Rgb i_color) {
    drawRectangle(i_x, i_y, i_size, i_size, i_color);
}

void Display::drawCircle(int i_x, int i_y, int radius, Rgb i_color) {
    // https://iq.opengenus.org/bresenhams-circle-drawing-algorithm/
    int x = 0;
    int y = radius;
    int decesionParameter = 3 - (2 * radius);

    displayBresenhmCircle(i_x, i_y, x, y, i_color);

    while (y >= x) {
        x++;
        if (decesionParameter > 0) {
            y--;
            decesionParameter = decesionParameter + 4 * (x - y) + 10;
        } else
            decesionParameter = decesionParameter + 4 * x + 6;
        displayBresenhmCircle(i_x, i_y, x, y, i_color);
    }
}

Rgb& Display::operator()(int i_x, int i_y) {
    return m_displayLeds[(i_y * m_width) + i_x];
}

Rgb& Display::operator[](int i_index) {
    return m_displayLeds[i_index];
}

void Display::show() {
    std::scoped_lock l(m_mutex);
    m_displayLeds.show();
    m_statusLeds.show();

    m_displayLeds.wait();
    m_statusLeds.wait();
}

void Display::show(int i_intensity) {
    for (int i = 0; i < m_width * m_height; i++)
        m_displayLeds[i].stretchChannelsEvenly(i_intensity);
    show();
}

Rgb& Display::pozice(int x, int y) {
    return at(x, y);
}

Rgb& Display::poziceStavovyRadek(int x) {
    return m_statusLeds[x];
}

Display::StatusBar& Display::statusBar() {
    return m_statusBar;
}

Display::StavovyRadek& Display::stavovyRadek() {
    return m_statusBar;
}

void Display::zacni() {
    init();
}

void Display::zapni() {
    turnOn();
}

void Display::vypni() {
    turnOff();
}

void Display::vycisti() {
    clear();
}

void Display::vycistiStavovyRadek() {
    clearStatusBar();
}

void Display::nakresliObdelnik(int x, int y, int sirka, int vyska, Rgb barva) {
    drawRectangle(x, y, sirka, vyska, barva);
}

void Display::nakresliCtverec(int x, int y, int strana, Rgb barva) {
    drawSquare(x, y, strana, barva);
}

void Display::nakresliKruznici(int stredX, int stredY, int polomer, Rgb barva) {
    drawCircle(stredX, stredY, polomer, barva);
}

void Display::ukaz() {
    show();
}

void Display::ukaz(int intenzita) {
    show(intenzita);
}
