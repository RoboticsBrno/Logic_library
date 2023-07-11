#include "StatusBar.hpp"

#include "Error.hpp"
#include "Platform.hpp"
#include <SmartLeds.h>
#include <mutex>

StatusBar::StatusBar(Rgb color, int count, int i_channel)
    : m_statusLeds(LED_WS2812, count, Platform::Pins::Status, i_channel) {
}

void StatusBar::setState(int state) {
    checkRange(state, 0, s_count, m_tag);
    m_state = state;
    for (int i = 0; i < state; i++)
        m_frame[i] = m_color;
}

int StatusBar::state() const {
    return m_state;
}

Rgb StatusBar::color() const {
    return m_color;
}

void StatusBar::setColor(Rgb color) {
    m_color = color;
}

void StatusBar::show(int i_intensity) {
    checkRange(i_intensity, 0, 255, m_tag);
    std::scoped_lock l(m_mutex);
    for (int i = 0; i < s_count; i++) {
        m_statusLeds[i] = m_frame[i];
        if (i_intensity != 255)
            m_statusLeds[i].stretchChannelsEvenly(i_intensity);
    }
    m_statusLeds.show();
    m_statusLeds.wait();
}

void StatusBar::clear() {
    for (int i = 0; i < m_state; i++)
        m_frame[i] = Rgb(0, 0, 0);
}

Rgb& StatusBar::at(int x) {
    checkRange(x, 0, s_count - 1, m_tag);
    return m_frame[x];
}

Rgb& StatusBar::operator[](int i_index) {
    return m_frame[i_index];
}
