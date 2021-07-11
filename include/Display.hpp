#pragma once

#include "Pinout.hpp"
#include <SmartLeds.h>
#include <mutex>

/**
 * @brief Structure for describing rectangles on display
 * 
 */
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

/**
 * @brief Struktura pro kreslení obdélníků
 */
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

    /**
     * @brief Returns reference to pixel on that position
     * 
     * @param x X coordinate 
     * @param y Y coordinate
     * @return Rgb& Pixel at requested position
     */
    Rgb& at(int x, int y);

    /**
     * @brief Vrátí pixel na dané pozici
     * 
     * @param x X-ová souřadnice
     * @param y Y-ová souřadnice
     * @return Rgb& Reference na pixel na dané pozici
     */
    Rgb& pozice(int x, int y) { return at(x, y); }

    /**
     * @brief Set the color of pixel at specified position
     * 
     * @param x X coordinate
     * @param y Y coordinate
     * @param color Color
     */
    inline void setColor(int x, int y, Rgb color) {
        if ((x > (m_width - 1)) || (y > (m_width - 1)) || (x < 0) || (y < 0))
            return;
        m_frame[(y * m_width) + x] = color;
    }

    /**
     * @brief Nastaví barvu pixelu na dané pozici
     * 
     * @param x X-ová souřadnice
     * @param y Y-ová souřadnice
     * @param color Barva
     */
    inline void nastavBarvu(int x, int y, Rgb color) { setColor(x, y, color); }

    /**
     * @brief Clear the display
     */
    void clear();

    /**
     * @brief Vyčistí celý displej
     */
    void vycisti() { clear(); }

    /**
     * @brief Draw rectangle with specified parameters 
     * 
     * @param x X coordinate of top-left corner
     * @param y Y coordinate of top-left corner
     * @param width 
     * @param height 
     * @param color 
     * @param strokeWidth 
     */
    void drawRectangle(int x, int y, int width, int height, Rgb color, int strokeWidth = 1);

    /**
     * @brief Draw rectangle with specified parameters
     * 
     * @param rect Struct used to define parameters
     * @param color 
     * @param strokeWidth 
     */
    void drawRectangle(const Rectangle& rect, Rgb color, int strokeWidth = 1) { drawRectangle(rect.x, rect.y, rect.w, rect.h, color, strokeWidth); }

    /**
     * @brief Draw filled rectangle with specified parameters
     * 
     * @param x X coordinate of top-left corner
     * @param y Y coordinate of top-left corner
     * @param width 
     * @param height 
     * @param color 
     */
    void drawRectangleFilled(int x, int y, int width, int height, Rgb color) {
        drawRectangle(x, y, width, height, color, INT_MAX);
    }

    /**
     * @brief Draw filled rectangle with specified parameters
     * 
     * @param rect Struct used to define parameters
     * @param color 
     */
    void drawRectangleFilled(const Rectangle& rect, Rgb color) {
        drawRectangle(rect.x, rect.y, rect.w, rect.h, color, INT_MAX);
    }

    /**
     * @brief Nakreslí obdélník se zadanými parametry
     * 
     * @param x X-ová souřadnice levého horního rohu obdélníku
     * @param y Y-ová souřadnice levého horního rohu obdélníku
     * @param sirka 
     * @param vyska 
     * @param barva 
     * @param tloustkaCary 
     */
    void nakresliObdelnik(int x, int y, int sirka, int vyska, Rgb barva, int tloustkaCary = 1) { drawRectangle(x, y, sirka, vyska, barva, tloustkaCary); }

    /**
     * @brief Nakreslí obdélník se zadanými parametry
     * 
     * @param obdelnik Struktura pro zadání parametrů
     * @param barva 
     * @param tloustkaCary 
     */
    void nakresliObdelnik(const Obdelnik& obdelnik, Rgb barva, int tloustkaCary = 1) { drawRectangle(obdelnik, barva, tloustkaCary); }

    /**
     * @brief Nakreslí vyplněný obdélník se zadanými parametry
     * 
     * @param x X-ová souřadnice levého horního rohu obdélníku
     * @param y Y-ová souřadnice levého horního rohu obdélníku
     * @param sirka 
     * @param vyska 
     * @param barva 
     */
    void nakresliObdelnikVyplneny(int x, int y, int sirka, int vyska, Rgb barva) { drawRectangleFilled(x, y, sirka, vyska, barva); }

    /**
     * @brief Nakreslí vyplněný obdélník se zadanými parametry
     * 
     * @param obdelnik Struktura pro zadání parametrů
     * @param barva 
     */
    void nakresliObdelnikVyplneny(const Obdelnik& obdelnik, Rgb barva) { drawRectangleFilled(obdelnik, barva); }

    /**
     * @brief Draw square with specified parameters
     * 
     * @param x X coordinate of top-left corner
     * @param y Y coordinate of top-left corner
     * @param size Length of side
     * @param color 
     * @param strokeWidth 
     */
    void drawSquare(int x, int y, int size, Rgb color, int strokeWidth = 1) {
        drawRectangle(x, y, size, size, color, strokeWidth);
    }

    /**
     * @brief Draw filled square with specified parameters
     * 
     * @param x X coordinate of top-left corner
     * @param y Y coordinate of top-left corner
     * @param size Length of side
     * @param color 
     */
    void drawSquareFilled(int x, int y, int size, Rgb color) { drawSquare(x, y, size, color, INT_MAX); }

    /**
     * @brief Nakreslí čtverec se zadanými parametry
     * 
     * @param x X-ová souřadnice levého horního rohu obdélníku
     * @param y Y-ová souřadnice levého horního rohu obdélníku
     * @param strana Délka strany
     * @param barva 
     * @param tlouskaCary 
     */
    void nakresliCtverec(int x, int y, int strana, Rgb barva, int tlouskaCary = 1) { drawSquare(x, y, strana, barva, tlouskaCary); }

    /**
     * @brief Nakreslí čtverec se zadanými parametry
     * 
     * @param x X-ová souřadnice levého horního rohu obdélníku
     * @param y Y-ová souřadnice levého horního rohu obdélníku
     * @param strana Délka strany 
     * @param barva 
     */
    void nakresliCtverecVyplneny(int x, int y, int strana, Rgb barva) {
        drawSquareFilled(x, y, strana, barva);
    }

    /**
     * @brief Draw circle with specified parameters
     * 
     * @param centerX X coordinate of the center
     * @param centerY Y coordinate of the center
     * @param radius 
     * @param color 
     */
    void drawCircle(int centerX, int centerY, int radius, Rgb color);

    /**
     * @brief Draw filled circle with specified parameters
     * 
     * @param centerX X coordinate of the center
     * @param centerY Y coordinate of the center
     * @param radius 
     * @param color 
     */
    void drawCircleFilled(int centerX, int centerY, int radius, Rgb color);

    /**
     * @brief Nakreslí kružnici s danými parametry
     * 
     * @param stredX X-ová souřadnice středu
     * @param stredY Y-ová souřadnice středu
     * @param polomer 
     * @param barva 
     */
    void nakresliKruznici(int stredX, int stredY, int polomer, Rgb barva) { drawCircle(stredX, stredY, polomer, barva); }

    /**
     * @brief Nakreslí vyplněnou kružnici (kruh) s danými parametry
     * 
     * @param stredX X-ová souřadnice středu
     * @param stredY Y-ová souřadnice středu
     * @param polomer 
     * @param barva 
     */
    void nakresliKruzniciVyplnenou(int stredX, int stredY, int polomer, Rgb barva) { drawCircleFilled(stredX, stredY, polomer, barva); }

    /**
     * @brief Draw line
     * 
     * @param x1 X coordinate of the beginning
     * @param y1 Y coordinate of the beginning
     * @param x2 X coordinate of the end
     * @param y2 Y coordinate of the end
     * @param color 
     * @param strokeWidth 
     */
    void drawLine(int x1, int y1, int x2, int y2, Rgb color, int strokeWidth = 1);

    /**
     * @brief Nakreslí čáru
     * 
     * @param x1 X-ová souřadnice počátku
     * @param y1 Y-ová souřadnice počátku
     * @param x2 X-ová souřadnice konce
     * @param y2 Y-ová souřadnice konce
     * @param barva 
     * @param tloustkaCary 
     */
    void nakresliCaru(int x1, int y1, int x2, int y2, Rgb barva, int tloustkaCary = 1) {
        drawLine(x1, y1, x2, y2, barva);
    }

    /**
     * @brief Show prepared frame on display
     * 
     * @param intensity [0-255] maximal intenzity 
     */
    void show(int intensity = 255);

    /**
     * @brief Vykresli připravený snímek na displeji
     * 
     * @param intenzita [0-255] maximální intenzita na jejíž hodnotu se barvy upraví
     */
    void ukaz(int intenzita = 255) { show(intenzita); }

    Rgb& operator()(int x, int y);
    Rgb& operator[](int index);
};

using Displej = Display;
