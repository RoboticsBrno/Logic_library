#include "Display.hpp"

#include "Error.hpp"
#include <driver/gpio.h>
#include <mutex>

void Display::displayBresenhmCircle(int i_xCenter, int i_yCenter, int i_x, int i_y, Rgb i_color) {
    setColor(i_xCenter + i_x, i_yCenter + i_y, i_color);
    setColor(i_xCenter - i_x, i_yCenter + i_y, i_color);
    setColor(i_xCenter + i_x, i_yCenter - i_y, i_color);
    setColor(i_xCenter - i_x, i_yCenter - i_y, i_color);
    setColor(i_xCenter + i_y, i_yCenter + i_x, i_color);
    setColor(i_xCenter - i_y, i_yCenter + i_x, i_color);
    setColor(i_xCenter + i_y, i_yCenter - i_x, i_color);
    setColor(i_xCenter - i_y, i_yCenter - i_x, i_color);
}

void Display::setColor(int x, int y, Rgb color) {
    if ((x > (m_width - 1)) || (y > (m_width - 1)) || (x < 0) || (y < 0))
        return;
    m_displayLeds[(y * m_width) + x] = color;
}

Display::Display(int i_width, int i_eight, int i_channelDisplay, int i_channelStatus)
    : m_displayLeds(LED_WS2812, i_width * i_eight, Pins::Display, i_channelDisplay)
    , m_width(i_width)
    , m_height(i_eight) {
}

Rgb& Display::at(int i_x, int i_y) {
    checkRange(i_x, 0, m_width - 1, m_tag);
    checkRange(i_y, 0, m_height - 1, m_tag);
    return m_displayLeds[(i_y * m_width) + i_x];
}

void Display::clear() {
    for (int i = 0; i < m_width * m_height; i++)
        m_displayLeds[i] = Rgb(0, 0, 0);
}

void Display::drawRectangle(int i_x, int i_y, int i_width, int i_height, Rgb i_color) {
    for (int y = i_y; y < i_y + i_height; y++)
        for (int x = i_x; x < i_x + i_width; x++)
            setColor(x, y, i_color);
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

void Display::show() {
    std::scoped_lock l(m_mutex);
    m_displayLeds.show();

    m_displayLeds.wait();
}

void Display::show(int i_intensity) {
    checkRange(i_intensity, 0, 255, m_tag);
    for (int i = 0; i < m_width * m_height; i++)
        m_displayLeds[i].stretchChannelsEvenly(i_intensity);
    show();
}

Rgb& Display::operator()(int i_x, int i_y) {
    return m_displayLeds[(i_y * m_width) + i_x];
}

Rgb& Display::operator[](int i_index) {
    return m_displayLeds[i_index];
}