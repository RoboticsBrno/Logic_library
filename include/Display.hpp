#pragma once

#include "Pinout.hpp"
#include <SmartLeds.h>
#include <mutex>

struct Rectangle {
    Rectangle(int x, int y, int w, int h)
        : x(x)
        , y(y)
        , w(w)
        , h(h) {
    }

    int x, y;
    int w, h;
};

typedef Rectangle Obdelnik;

class Display {
    friend class Logic;

private:
    SmartLed m_displayLeds;

    std::mutex m_mutex;

    const char* m_tag = "Display";

    static constexpr int m_width = 10;
    static constexpr int m_height = 10;
    std::array<Rgb, m_width * m_height> m_frame;

    void displayBresenhmCircle(int xCenter, int yCenter, int x, int y, Rgb color);

    Display();

public:
    Display(Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(Display&) = delete;
    Display& operator=(Display&&) = delete;

    ~Display() = default;

    Rgb& at(int x, int y);
    Rgb& pozice(int x, int y) { return at(x, y); }

    inline void setColor(int x, int y, Rgb color) {
        if ((x > (m_width - 1)) || (y > (m_width - 1)) || (x < 0) || (y < 0))
            return;
        m_frame[(y * m_width) + x] = color;
    }

    inline void nastavBarvu(int x, int y, Rgb color) { setColor(x, y, color); }

    void clear();
    void vycisti() { clear(); }

    void drawRectangle(int x, int y, int width, int height, Rgb color, int strokeWidth = 1);
    void drawRectangle(const Rectangle& rect, Rgb color, int strokeWidth = 1) { drawRectangle(rect.x, rect.y, rect.w, rect.h, color, strokeWidth); }
    void drawRectangleFilled(int x, int y, int width, int height, Rgb color) {
        drawRectangle(x, y, width, height, color, INT_MAX);
    }
    void drawRectangleFilled(const Rectangle& rect, Rgb color) {
        drawRectangle(rect.x, rect.y, rect.w, rect.h, color, INT_MAX);
    }
    void nakresliObdelnik(int x, int y, int sirka, int vyska, Rgb barva, int tloustkaCary = 1) { drawRectangle(x, y, sirka, vyska, barva, tloustkaCary); }
    void nakresliObdelnik(const Obdelnik& obdelnik, Rgb barva, int tloustkaCary = 1) { drawRectangle(obdelnik, barva, tloustkaCary); }
    void nakresliObdelnikVyplneny(int x, int y, int sirka, int vyska, Rgb barva) { drawRectangleFilled(x, y, sirka, vyska, barva); }
    void nakresliObdelnikVyplneny(const Obdelnik& obdelnik, Rgb barva) { drawRectangleFilled(obdelnik, barva); }

    void drawSquare(int x, int y, int size, Rgb color, int strokeWidth = 1) {
        drawRectangle(x, y, size, size, color, strokeWidth);
    }
    void drawSquareFilled(int x, int y, int size, Rgb color) { drawSquare(x, y, size, color, INT_MAX); }
    void nakresliCtverec(int x, int y, int strana, Rgb barva, int tlouskaCary = 1) { drawSquare(x, y, strana, barva, tlouskaCary); }
    void nakresliCtverecVyplneny(int x, int y, int strana, Rgb barva) {
        drawSquareFilled(x, y, strana, barva);
    }

    void drawCircle(int centerX, int centerY, int radius, Rgb color);
    void drawCircleFilled(int centerX, int centerY, int radius, Rgb color);
    void nakresliKruznici(int stredX, int stredY, int polomer, Rgb barva) { drawCircle(stredX, stredY, polomer, barva); }
    void nakresliKruzniciVyplnenou(int stredX, int stredY, int polomer, Rgb barva) { drawCircleFilled(stredX, stredY, polomer, barva); }

    void drawLine(int x1, int y1, int x2, int y2, Rgb color, int strokeWidth = 1);
    void nakresliCaru(int x1, int y1, int x2, int y2, Rgb barva, int tloustkaCary = 1) {
        drawLine(x1, y1, x2, y2, barva);
    }

    void show(int intensity = 255);
    void ukaz(int intenzita = 255) { show(intenzita); }

    Rgb& operator()(int x, int y);
    Rgb& operator[](int index);
};

using Displej = Display;
