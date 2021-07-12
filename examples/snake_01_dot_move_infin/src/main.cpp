#include "Logic.hpp"
#include <iostream>

const int ledIndexMax = 9;

int gamePosX = 5;
int gamePosY = 5;

void showGamePos() {
    logic.display().clear();
    logic.display().at(gamePosX, gamePosY) = Rgb(255, 255, 255);
    logic.display().show(50);
}

void logicMain() {
    logic.buttons().onPress([]() {
        std::cout << "Button UP" << std::endl;
        gamePosY--;
        if (gamePosY < 0) {
            gamePosY = ledIndexMax;
        }
    },
        Up);

    logic.buttons().onPress([]() {
        std::cout << "Button DOWN" << std::endl;
        gamePosY++;
        if (gamePosY > ledIndexMax) {
            gamePosY = 0;
        }
    },
        Down);

    logic.buttons().onPress([]() {
        std::cout << "Button LEFT" << std::endl;
        gamePosX--;
        if (gamePosX < 0) {
            gamePosX = ledIndexMax;
        }
    },
        Left);

    logic.buttons().onPress([]() {
        std::cout << "Button RIGHT" << std::endl;
        gamePosX++;
        if (gamePosX > ledIndexMax) {
            gamePosX = 0;
        }
    },
        Right);

    while (true) {
        showGamePos();
        std::cout << "GamePos[" << gamePosX << ", " << gamePosY << "]" << std::endl;
    }
}