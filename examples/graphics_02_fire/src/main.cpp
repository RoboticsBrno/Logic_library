#include "Logic.hpp"
#include <iostream>
#include <array>

const int MAX_TEMPERATURE = 33;

std::array< Rgb, MAX_TEMPERATURE + 1 > temperatures{
    Rgb(0, 0, 0),
    Rgb(10, 7, 10),
    Rgb(31, 7, 7),
    Rgb(47, 15, 7),
    Rgb(71, 15, 7),
    Rgb(87, 23, 7),
    Rgb(103, 31, 7),
    Rgb(119, 31, 7),
    Rgb(143, 39, 7),
    Rgb(159, 47, 7),
    Rgb(175, 63, 7),
    Rgb(191, 71, 7),
    Rgb(199, 71, 7),
    Rgb(223, 79, 7),
    Rgb(223, 87, 7),
    Rgb(223, 87, 7),
    Rgb(215, 95, 7),
    Rgb(215, 95, 7),
    Rgb(215, 103, 15),
    Rgb(207, 111, 15),
    Rgb(207, 119, 15),
    Rgb(207, 120, 15),
    Rgb(207, 120, 23),
    Rgb(199, 120, 23),
    Rgb(199, 125, 23),
    Rgb(199, 125, 31),
    Rgb(191, 130, 31),
    Rgb(191, 130, 31),
    Rgb(191, 130, 39),
    Rgb(191, 135, 47),
    Rgb(183, 135, 47),
    Rgb(183, 140, 47),
    Rgb(183, 140, 55),
    Rgb(255, 255, 255)
};

std::array< std::array< int, 10>, 11 > firePixels;

void seedFire() {
    for (int x = 0; x != 10; x++)
        firePixels[x][10] = MAX_TEMPERATURE;
}

void stopFire() {
    for (int x = 0; x != 10; x++)
        firePixels[x][10] = 0;
}

void spreadFire(int x, int y) {
    int newTemperature = firePixels[x][y] - random(1, 9);
    if (newTemperature < 0)
        newTemperature = 0;
    int newX = x + random(-2, 1);
    if (newX < 0 || newX >= 10)
        return;
    firePixels[newX][y - 1] = newTemperature;
}

void doFire() {
    for(int x = 0; x < 10; x++) {
        for (int y = 1; y < 11; y++) {
            spreadFire(x, y);
        }
    }
}

void showFire() {
    for(int x = 0; x < 10; x++) {
        for (int y = 1; y < 10; y++) {
            Rgb c = temperatures[firePixels[x][y]];
            c.linearize();
            display.setColor(x, y, c);
        }
    }
}

void logicMain() {
    bool running = true;
    buttons.onPress([&](){
        running = !running;
    });

    while( true ) {
        if (running)
            seedFire();
        else
            stopFire();
        doFire();
        showFire();
        display.show();

        delay(50);
    }
}