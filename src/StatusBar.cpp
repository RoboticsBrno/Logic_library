#include "StatusBar.hpp"

#include "Error.hpp"
#include "Pinout.hpp"
#include <SmartLeds.h>
#include <mutex>

StatusBar::StatusBar(Rgb color, int count, int i_channel)
    : m_statusLeds(LED_WS2812, count, Pins::Status, i_channel)
    , m_count(count)
    , m_state(0)
    , m_color(color) {
}

void StatusBar::setState(int state) {
    checkRange(state, 0, 5, m_tag);
    m_state = state;
    render();
}

int StatusBar::state() const {
    return m_state;
}

Rgb StatusBar::color() const {
    return m_color;
}

void StatusBar::setColor(Rgb color) {
    m_color = color;
    render();
}

StatusBar& StatusBar::operator+=(int other) {
    sanityCheck(m_state < (6 - other), m_tag, VariableOverflow);
    m_state += other;
    render();
    return *this;
}

StatusBar& StatusBar::operator-=(int other) {
    sanityCheck(m_state > other, m_tag, VariableUnderflow);
    m_state -= other;
    render();
    return *this;
}

StatusBar& StatusBar::operator++() {
    sanityCheck(m_state < 5, m_tag, VariableOverflow);
    m_state++;
    render();
    return *this;
}

StatusBar& StatusBar::operator--() {
    sanityCheck(m_state > 0, m_tag, VariableUnderflow);
    m_state--;
    render();
    return *this;
}

int StatusBar::operator++(int other) {
    sanityCheck(m_state < 5, m_tag, VariableOverflow);
    int out = m_state;
    m_state++;
    render();
    return out;
}

int StatusBar::operator--(int other) {
    sanityCheck(m_state > 0, m_tag, VariableUnderflow);
    int out = m_state;
    m_state--;
    render();
    return out;
}

StatusBar::operator int() const {
    return m_state;
}

StatusBar& StatusBar::operator=(int state) {
    checkRange(state, 0, 5, m_tag);
    m_state = state;
    render();
    return *this;
}

void StatusBar::show() {
    m_statusLeds.show();
    m_statusLeds.wait();
}

void StatusBar::show(int i_intensity) {
    checkRange(i_intensity, 0, 255, m_tag);
    for (int i = 0; i < m_count; i++)
        m_statusLeds[i].stretchChannelsEvenly(i_intensity);
    show();
}

void StatusBar::render() {
    for (int i = 0; i <= m_state; i++)
        m_statusLeds[i] = m_color;
    show();
}

void StatusBar::render(int i_intensity) {
    for (int i = 0; i <= m_state; i++)
        m_statusLeds[i] = m_color;
    show(i_intensity);
}

void StatusBar::clear() {
    for (int i = 0; i <= m_state; i++)
        m_statusLeds[i] = Rgb(0, 0, 0);
}

Rgb& StatusBar::at(int x) {
    return m_statusLeds[x];
}
