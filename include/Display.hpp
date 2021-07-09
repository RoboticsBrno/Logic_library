#pragma once

#include "Pinout.hpp"
#include <SmartLeds.h>
#include <mutex>

enum States {
    A = 0,
    B,
    C,
    D,
    E,
};

using Stavy = States;

class Display {
private:
    SmartLed m_displayLeds;
    SmartLed m_statusLeds;

    mutable std::mutex m_mutex;

    const char*  m_tag = "Display";

    const int m_width;
    const int m_height;

    void displayBresenhmCircle(int xCenter, int yCenter, int x, int y, Rgb color);

    class StatusBar {
    private:
        const char* m_tag = "StatusBar";
        Display& m_parent;
        int m_state;
        Rgb m_color;

        void render();

    public:
        StatusBar() = delete;
        StatusBar(StatusBar&) = delete;
        StatusBar(StatusBar&&) = delete;
        StatusBar& operator=(StatusBar&) = delete;
        StatusBar& operator=(StatusBar&&) = delete;

        StatusBar(Display& parent, Rgb color);

        void setState(int state);
        void nastavStav(int stav);
        int state() const;
        int stav() const;

        void setColor(Rgb color);
        void nastavBarvu(Rgb barva);
        Rgb color() const;
        Rgb barva() const;

        StatusBar& operator+=(int);
        StatusBar& operator-=(int);

        StatusBar& operator++();
        StatusBar& operator--();

        int operator++(int);
        int operator--(int);

        operator int() const;
        StatusBar& operator=(int state);
    } m_statusBar;

public:
    using StavovyRadek = StatusBar;
    Display(int width = 10, int height = 10, int channelDisplay = 0, int channelStatus = 1);
    ~Display() = default;

    Rgb& at(int x, int y);
    Rgb& atStatusBar(int x);

    StatusBar& statusBar();

    void init();
    void turnOn();
    void turnOff();

    void clear();
    void clearStatusBar();

    void drawRectangle(int x, int y, int width, int height, Rgb color);
    void drawSquare(int x, int y, int size, Rgb color);
    void drawCircle(int centerX, int centerY, int radius, Rgb color);

    Rgb& operator()(int x, int y);
    Rgb& operator[](int index);

    void show();
    void show(int intensity);

    Rgb& pozice(int x, int y);
    Rgb& poziceStavovyRadek(int x);

    StavovyRadek& stavovyRadek();

    void zacni();
    void zapni();
    void vypni();

    void vycisti();
    void vycistiStavovyRadek();

    void nakresliObdelnik(int x, int y, int sirka, int vyska, Rgb barva);
    void nakresliCtverec(int x, int y, int strana, Rgb barva);
    void nakresliKruznici(int stredX, int stredY, int polomer, Rgb barva);

    void ukaz();
    void ukaz(int intenzita);
};

using Displej = Display;
