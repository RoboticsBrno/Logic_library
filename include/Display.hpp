#pragma once

#include "Pinout.hpp"
#include <SmartLeds.h>
#include <mutex>

class Display {
    friend class Logic;

private:
    SmartLed m_displayLeds;

    std::mutex m_mutex;

    const char* m_tag = "Display";

    const int m_width;
    const int m_height;

    void displayBresenhmCircle(int xCenter, int yCenter, int x, int y, Rgb color);

    Display(int width = 10, int height = 10, int channelDisplay = 0, int channelStatus = 1);

public:
    Display(Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(Display&) = delete;
    Display& operator=(Display&&) = delete;

    ~Display() = default;

    Rgb& at(int x, int y);
    Rgb& pozice(int x, int y) { return at(x, y); }

    void setColor(int x, int y, Rgb color);
    void nastavBarvu(int x, int y, Rgb color) { setColor(x, y, color); }

    void clear();
    void vycisti() { clear(); }

    void drawRectangle(int x, int y, int width, int height, Rgb color);
    void nakresliObdelnik(int x, int y, int sirka, int vyska, Rgb barva) { drawRectangle(x, y, sirka, vyska, barva); }

    void drawSquare(int x, int y, int size, Rgb color);
    void nakresliCtverec(int x, int y, int strana, Rgb barva) { drawSquare(x, y, strana, barva); }

    void drawCircle(int centerX, int centerY, int radius, Rgb color);
    void nakresliKruznici(int stredX, int stredY, int polomer, Rgb barva) { drawCircle(stredX, stredY, polomer, barva); }

    void show();
    void ukaz() { show(); }

    void show(int intensity);
    void ukaz(int intenzita) { show(intenzita); }

    Rgb& operator()(int x, int y);
    Rgb& operator[](int index);
};

using Displej = Display;
