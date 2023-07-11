#pragma once

#include "Platform.hpp"
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
     * @brief Returns width of the display in pixels
     * @return int width of the display in pixels
     */
    int width() const { return m_width; }
    /**
     * @brief Returns height of the display in pixels
     * @return int height of the display in pixels
     */
    int height() const { return m_height; }

    /**
     * @brief Vrací šířku displeje v pixelech.
     * @return int šířka displeje v pixelech
     */
    int sirka() const { return m_width; }
    /**
     * @brief Vrací výšku displeje v pixelech.
     * @return int výška displeje v pixelech.
     */
    int vyska() const { return m_height; }

    /**
     * @brief Returns width of each character in the font used by drawCharacter and drawString.
     * @return int width of one character in pixels.
     */
    int fontWidth() const;
    /**
     * @brief Returns height of each character in the font used by drawCharacter and drawString.
     * @return int height of one character in pixels.
     */
    int fontHeight() const;

    /**
     * @brief Vrátí šířku jednoho znaku z písma použitého v metodách nakresliZnak a nakresliText.
     * @return int šířka jednoho znaku v pixelech.
     */
    int sirkaPisma() const { return fontWidth(); }
    /**
     * @brief Vrátí výšku jednoho znaku z písma použitého v metodách nakresliZnak a nakresliText.
     * @return int výšku jednoho znaku v pixelech.
     */
    int vyskaPisma() const { return fontHeight(); }

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
    void clear() {
        fill(Rgb(0, 0, 0));
    }

    /**
     * @brief Vyčistí celý displej
     */
    void vycisti() { clear(); }

    /**
     * @brief Fill the display with color
     *
     * @param color Color
     */
    void fill(Rgb color);

    /**
     * @brief Vyplň celý displej barvou.
     *
     * @param barva barva
     */
    void vypln(Rgb barva) {
        fill(barva);
    }

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
    void
    drawRectangle(int x, int y, int width, int height, Rgb color, int strokeWidth = 1);

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
     * @brief Draw a single character to the display. The dimensions of the characters can be obtained by calling
     * fontWidth() and fontHeight().
     *
     * @param c which character to draw, from ASCII + ISO-8859-1 codepage.
     * @param color which color to use
     * @param offsetX X offset where to start the drawing.
     * @param offsetY Y offset where to start the drawing.
     */
    void drawCharacter(char c, Rgb color, int offsetX = 2, int offsetY = 0);

    /**
     * @brief Nakreslí jeden znak na displej. Velikost znaků dostanete z metod sirkaPisma() a vyskaPisma().
     *
     * @param znak který znak vykreslit, z ASCII + ISO-8859-1 kódové stránky.
     * @param barva barva znaku
     * @param posunX o kolik pixelů v X souřadnicích posunout znak doprava.
     * @param posunY o kolik pixelů v Y souřadnicích posunout znak dolů.
     */
    void nakresliZnak(char znak, Rgb barva, int posunX = 2, int posunY = 0) {
        drawCharacter(znak, barva, posunX, posunY);
    }

    /**
     * @brief Draws whole string to the display, handling Czech UTF-8 sequences correctly.
     *
     * @param utf8Czech String to draw on the display
     * @param color which color to use
     * @param offsetX X offset where to start the drawing. This offsets the whole string, which means you can use negative offset
     *                to scroll and show the whole string.
     * @param offsetY Y offset where to start the drawing.
     * @return int number of separate drawable characters (after UTF-8 decoding).
     */
    int drawString(const char* utf8Czech, Rgb color, int offsetX = 2, int offsetY = 0);

    /**
     * @see Display::drawString
     */
    int drawString(const std::string& utf8Czech, Rgb color, int offsetX = 2, int offsetY = 0) {
        return drawString(utf8Czech.c_str(), color, offsetX, offsetY);
    }

    /**
     * @brief Nakreslí na displej textový řetězec, včetně český znaků v UTF-8.
     *
     * @param utf8CeskyText řetězec na vykreslení
     * @param barva barva textu
     * @param posunX na které X pozici začít kreslit text. Může být záporný, tedy lze využít na implementaci posuvu textu
     *               a vykreslení celého textu.
     * @param posunY na které Y pozici kreslit text.
     * @return int počet dekódovaných vykreslitelných znaků
     */
    int nakresliText(const char* utf8CeskyText, Rgb barva, int posunX = 2, int posunY = 0) {
        return drawString(utf8CeskyText, barva, posunX, posunY);
    }

    /**
     * @see Display::nakresliText
     */
    int nakresliText(const std::string& utf8CeskyText, Rgb barva, int posunX = 2, int posunY = 0) {
        return drawString(utf8CeskyText, barva, posunX, posunY);
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

static const Rgb Black(0, 0, 0);
static const Rgb White(255, 255, 255);
static const Rgb Red(255, 0, 0);
static const Rgb Green(0, 255, 0);
static const Rgb Blue(0, 0, 255);
static const Rgb Yellow(255, 255, 0);
static const Rgb Cyan(0, 255, 255);
static const Rgb Magenta(255, 0, 255);
static const Rgb Purple(128, 0, 128);
static const Rgb Pink(255, 30, 150);

static const Rgb Cerna(0, 0, 0);
static const Rgb Bila(255, 255, 255);
static const Rgb Cervena(255, 0, 0);
static const Rgb Zelena(0, 255, 0);
static const Rgb Modra(0, 0, 255);
static const Rgb Zluta(255, 255, 0);
static const Rgb Azurova(0, 255, 255);
static const Rgb Fialova(255, 0, 255);
static const Rgb Ruzova(255, 30, 150);
