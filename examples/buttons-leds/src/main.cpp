#include "Logic.hpp"
#include <iostream>

void logicMain() {
    while (true) {
        display.clear();

        if(buttons.read(Enter)) {
            display.drawSquareFilled(3, 3, 4, Rgb(255, 255, 255));    
        }
        else if(buttons.read(Right)) {
            display.drawSquareFilled(6, 3, 4, Rgb(255, 255, 255));
        }
        else if(buttons.read(Down)) {
            display.drawSquareFilled(3, 6, 4, Rgb(255, 255, 255));
        }
        else if(buttons.read(Left)) {
            display.drawSquareFilled(0, 3, 4, Rgb(255, 255, 255));
        }
        else if(buttons.read(Up)) {
            display.drawSquareFilled(3, 0, 4, Rgb(255, 255, 255));
        }
        display.show(30);
    }
}