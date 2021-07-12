#include "Logic.hpp"
#include <iostream>

void logicMain() {
    while (true) {
        statusBar.setColor(Rgb(255, 0, 0)); // red
        statusBar.setState(4);

        statusBar.show(30);
        delay(3000);
        statusBar.clear();

        statusBar.setColor(Rgb(0, 255, 0)); // red
        statusBar.setState(2);

        statusBar.show(30);
        delay(3000);
        statusBar.clear();

        statusBar.at(4) = Rgb(0, 0, 255);

        statusBar.show(30);
        delay(3000);
        statusBar.clear();

        statusBar.at(0) = Rgb(255, 0, 0);
        statusBar.at(1) = Rgb(0, 255, 0);
        statusBar.at(2) = Rgb(0, 0, 255);

        statusBar.show(30);
        delay(3000);
        statusBar.clear();
    }
}