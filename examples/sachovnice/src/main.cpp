#include "Logic.hpp"
#include <iostream>

bool jeSude(int x) {
    return x % 2 == 0;
}

void radekSachovnice(int y, Rgb prvni, Rgb druha) {
    for(int x = 0; x < 10; x = x + 1) {
        if (jeSude(x)) {
            display.setColor(x, y, prvni);
        }
        else {
            display.setColor(x, y, druha);
        }
    }
}

void vykresliSachovnici(Rgb prvni, Rgb druha) {
    for(int y = 0; y < 10; y = y + 1) {
        if (jeSude(y)) {
            radekSachovnice(y, prvni, druha);
        }
        else {
            radekSachovnice(y, prvni, druha);
        }
    }
}

void logicMain() {
    vykresliSachovnici(Rgb(255, 0, 0), Rgb(0, 0, 255));
    display.show();
}