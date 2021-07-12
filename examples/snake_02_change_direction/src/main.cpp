#include "Logic.hpp"
#include <iostream>

const int ledIndexMax = 9;

int gamePosX = 5;
int gamePosY = 5;

int gameDirection = Right;

void moveGamePos() {
    switch (gameDirection) {
    case Up:
        gamePosY--;
        if (gamePosY < 0) {
            gamePosY = ledIndexMax;
        }
        break;
    case Down:
        gamePosY++;
        if (gamePosY > ledIndexMax) {
            gamePosY = 0;
        }
        break;
    case Left:
        gamePosX--;
        if (gamePosX < 0) {
            gamePosX = ledIndexMax;
        }
        break;
    case Right:
        gamePosX++;
        if (gamePosX > ledIndexMax) {
            gamePosX = 0;
        }
        break;
    }
}

void showGamePos() {
    logic.display().clear();
    logic.display().at(gamePosX, gamePosY) = Rgb(255, 255, 255);
    logic.display().show(50);
}

void logicMain() {
    logic.buttons().onPress([]() {
        std::cout << "Button UP" << std::endl;
        gameDirection = Up;
    },
        Up);

    logic.buttons().onPress([]() {
        std::cout << "Button DOWN" << std::endl;
        gameDirection = Down;
    },
        Down);

    logic.buttons().onPress([]() {
        std::cout << "Button LEFT" << std::endl;
        gameDirection = Left;
    },
        Left);

    logic.buttons().onPress([]() {
        std::cout << "Button RIGHT" << std::endl;
        gameDirection = Right;
    },
        Right);

    while (true) {
        moveGamePos();
        showGamePos();
        std::cout << "GamePos[" << gamePosX << ", " << gamePosY << "]" << std::endl;
        delay(400);
    }
}