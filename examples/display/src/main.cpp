#include "Logic.hpp"
#include <iostream>

void logicMain() {
    while (true)
    {
        display.fill(Rgb(255, 0, 0)); //red
        
        display.show(30);
        delay(3000);
        display.clear();

        display.drawLine(1, 0, 8, 0, Rgb(255, 0, 0)); // red
        display.drawLine(0, 1, 8, 9, Rgb(0, 255, 0)); // green

        display.show(30);
        delay(3000);
        display.clear();

        display.drawRectangle(0, 1, 8, 4, Rgb(255, 0, 0)); // red
        display.drawRectangle(4, 3, 6, 5, Rgb(0, 255, 0)); // green
        
        display.show(30);
        delay(3000);
        display.clear();

        display.drawRectangleFilled(0, 1, 8, 4, Rgb(255, 0, 0)); // red
        display.drawRectangleFilled(4, 3, 6, 5, Rgb(0, 255, 0)); // green

        display.show(30);
        delay(3000);
        display.clear();

        display.drawCircle(3, 3, 2, Rgb(255, 0, 0)); // red
        display.drawCircle(6, 7, 3, Rgb(0, 255, 0)); // green

        display.show(30);
        delay(3000);
        display.clear();

        display.drawCircleFilled(3, 3, 2, Rgb(255, 0, 0)); // red
        display.drawCircleFilled(6, 7, 3, Rgb(0, 255, 0)); // green   

        display.show(30);
        delay(3000);
        display.clear();

        display.drawSquare(1, 1, 3, Rgb(255, 0, 0)); //red
        display.drawSquare(4, 5, 4, Rgb(0, 255, 0)); // green

        display.show(30);
        delay(3000);
        display.clear();

        display.drawSquareFilled(1, 1, 3, Rgb(255, 0, 0)); //red
        display.drawSquareFilled(4, 5, 4, Rgb(0, 255, 0)); // green

        display.show(30);
        delay(3000);
        display.clear();

        display.at(0, 0) = Rgb(255, 0, 0); // red
        display.at(4, 7) = Rgb(0, 255, 0); // green

    }
    

}