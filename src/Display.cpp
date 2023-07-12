#include <driver/gpio.h>
#include <math.h>
#include <mutex>

#include "Display.hpp"
#include "DisplayFont.hpp"
#include "Error.hpp"

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

Display::Display()
    : m_displayLeds(LED_WS2812B, m_width * m_height, Platform::Pins::Display, 0) {
}

int Display::fontWidth() const {
    return DisplayFont::DIMENSIONS_X;
}

int Display::fontHeight() const {
    return DisplayFont::DIMENSIONS_Y;
}

Rgb& Display::at(int i_x, int i_y) {
    checkRange(i_x, 0, m_width - 1, m_tag);
    checkRange(i_y, 0, m_height - 1, m_tag);
    return m_frame[(i_y * m_width) + i_x];
}

void Display::fill(Rgb color) {
    for (int i = 0; i < m_width * m_height; i++)
        m_frame[i] = color;
}

void Display::drawRectangle(int i_x, int i_y, int i_width, int i_height, Rgb i_color, int strokeWidth) {
    const int end_x = (i_x + i_width);
    const int end_y = (i_y + i_height);
    for (int y = i_y; y < end_y; y++) {
        for (int x = i_x; x < end_x; x++) {
            if (y - i_y < strokeWidth || end_y - y - 1 < strokeWidth || x - i_x < strokeWidth || end_x - x - 1 < strokeWidth) {
                setColor(x, y, i_color);
            }
        }
    }
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

void Display::drawCircleFilled(int centerX, int centerY, int radius, Rgb color) {
    const int radius_check = radius * radius + radius * 0.8;

    for (int ry = -radius; ry <= radius; ++ry)
        for (int rx = -radius; rx <= radius; ++rx)
            if (rx * rx + ry * ry <= radius_check)
                setColor(centerX + rx, centerY + ry, color);
}

void Display::drawLine(int x1, int y1, int x2, int y2, Rgb color, int strokeWidth) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    const int sx = x1 < x2 ? 1 : -1;
    const int sy = y1 < y2 ? 1 : -1;

    int err;
    float e2 = sqrtf((float)(dx * dx) + (float)(dy * dy));

    if (e2 == 0.0)
        return;

    dx *= 255 / e2;
    dy *= 255 / e2;
    float th = 255 * ((float)(strokeWidth) + 1);

    if (dx < dy) {
        x2 = (e2 + th / 2) / dy;
        err = x2 * dy - th / 2;
        for (x1 -= x2 * sx;; y1 += sy) {
            x2 = x1;
            for (e2 = dy - err - th; e2 + dy < 255; e2 += dy) {
                x2 += sx;
                setColor(x2, y1, color);
            }
            if (y1 == y2)
                break;
            err += dx;
            if (err > 255) {
                err -= dy;
                x1 += sx;
            }
        }
    } else {
        y2 = (e2 + th / 2) / dx;
        err = y2 * dx - th / 2;
        for (y1 -= y2 * sy;; x1 += sx) {
            y2 = y1;
            for (e2 = dx - err - th; e2 + dx < 255; e2 += dx) {
                y2 += sy;
                setColor(x1, y2, color);
            }

            if (x1 == x2)
                break;
            err += dy;
            if (err > 255) {
                err -= dx;
                y1 += sy;
            }
        }
    }
}

void Display::drawCharacter(char c, Rgb color, int offsetX, int offsetY) {
    const auto fontChar = DisplayFont::get(c);
    for (int y = 0; y < DisplayFont::DIMENSIONS_Y; ++y) {
        for (int x = 0; x < DisplayFont::DIMENSIONS_X; ++x) {
            if (fontChar(x, y)) {
                setColor(x + offsetX, y + offsetY, color);
            }
        }
    }
}

int Display::drawString(const char* utf8Czech, Rgb color, int offsetX, int offsetY) {
    const int paddedCharW = DisplayFont::DIMENSIONS_X + 1;

    int drawnCounter = 0;
    for (const char* c = utf8Czech; *c; ++c) {
        uint8_t displayChar = *c;
        if (displayChar > 128) {
            displayChar = DisplayFont::utf8toCp(c);
            while (((uint8_t)*c) > 128)
                ++c;
            --c;
        }

        const int drawOffX = offsetX;
        offsetX += paddedCharW;
        ++drawnCounter;

        if (drawOffX <= -((int)DisplayFont::DIMENSIONS_X) || drawOffX >= m_width) {
            continue;
        }

        if (displayChar != 0) {
            drawCharacter(displayChar, color, drawOffX, offsetY);
        }
    }
    return drawnCounter;
}

void Display::show(int i_intensity) {
    checkRange(i_intensity, 0, 255, m_tag);
    std::scoped_lock l(m_mutex);
    for (size_t i = 0; i < m_frame.size(); ++i) {
        m_displayLeds[i] = m_frame[i];
        if (i_intensity != 255) {
            m_displayLeds[i].stretchChannelsEvenly(i_intensity);
        }
    }
    m_displayLeds.show();
    m_displayLeds.wait();
}

Rgb& Display::operator()(int i_x, int i_y) {
    return at(i_x, i_y);
}

Rgb& Display::operator[](int i_index) {
    checkRange(i_index, 0, m_frame.size() - 1, m_tag);
    return m_frame[i_index];
}
