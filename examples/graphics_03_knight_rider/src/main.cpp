#include "Logic.hpp"
#include <iostream>

const Rgb baseColor(255, 0, 0);

void showDot(int x, int y, float coefficient = 1) {
    coefficient = coefficient * coefficient;
    display.setColor(x, y, Rgb(
        coefficient * baseColor.r,
        coefficient * baseColor.g,
        coefficient * baseColor.b));

    display.setColor(x - 1, y, Rgb(
        coefficient * baseColor.r / 4,
        coefficient * baseColor.g / 4,
        coefficient * baseColor.b / 4));
    display.setColor(x - 2, y, Rgb(
        coefficient * baseColor.r / 8,
        coefficient * baseColor.g / 8,
        coefficient * baseColor.b / 8));

    display.setColor(x + 1, y, Rgb(
        coefficient * baseColor.r / 4,
        coefficient * baseColor.g / 4,
        coefficient * baseColor.b / 4));
    display.setColor(x + 2, y, Rgb(
        coefficient * baseColor.r / 8,
        coefficient * baseColor.g / 8,
        coefficient * baseColor.b / 8));
}

void logicMain() {
    const int leftMantinel = 3;
    const int rightMantinel = 6;

    int position = leftMantinel;
    int direction = 1;

    while (true) {
        display.clear();

        for (int y = 0; y < 10; y++) {
            showDot(position, y, 1 - abs(y - 5) / 6.0);
        }

        display.show();

        position += direction;
        if (position > rightMantinel || position < leftMantinel)
            direction = -direction;
        delay(100);
    }
}