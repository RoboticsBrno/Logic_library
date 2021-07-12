#include "Logic.hpp"
#include <iostream>
#include <vector>

struct Position {
    int x;
    int y;
};

const int ledIndexMax = 9;
int gameDirection = Right;

Position actualPos;
Position foodPos;

std::vector<Position> snakePos;

bool isInSamePlace(Position first, Position second) {
    return first.x == second.x && first.y == second.y;
}

void generateFeed() {
    std::vector<Position> free;

    for (int i = 0; i < 100 - snakePos.size(); i++) {
        bool check = true;
        Position pos = {
            .x = i % 10,
            .y = i / 10,
        };
        for (int i = 0; i < snakePos.size(); i++) {
            check *= !isInSamePlace(snakePos[i], pos);
            if (!check)
                break;
        }
        if (check)
            free.push_back(pos);
    }
    foodPos = free[random(0, free.size() - 1)];
}

void moveGamePos() {
    std::cout << "-gdD: " << gameDirection;
    switch (gameDirection) {
    case Up:
        actualPos.y--;
        if (actualPos.y < 0) {
            actualPos.y = ledIndexMax;
        }
        break;
    case Down:
        actualPos.y++;
        if (actualPos.y > ledIndexMax) {
            actualPos.y = 0;
        }
        break;
    case Left:
        actualPos.x--;
        if (actualPos.x < 0) {
            actualPos.x = ledIndexMax;
        }
        break;
    case Right:
        actualPos.x++;
        if (actualPos.x > ledIndexMax) {
            actualPos.x = 0;
        }
        break;
    }

    if (isInSamePlace(snakePos[0], foodPos)) {
        std::cout << "-Eat";
        snakePos.push_back(actualPos);
        generateFeed();
    } else {
        std::cout << "-noEat";
        for (int i = 0; i < snakePos.size() - 1; i++) {
            snakePos[i] = snakePos[i + 1];
        }
        snakePos[snakePos.size() - 1] = actualPos;
    }
}

bool isDead() {
    for (int i = 0; i < snakePos.size() - 1; i++) {
        if (isInSamePlace(snakePos[i], actualPos)) {
            std::cout << "-Dead";
            return true;
        }
    }
    std::cout << "-noDead";
    return false;
}

void showGamePos() {
    std::cout << "-showGame";
    logic.display().clear();

    logic.display().at(foodPos.x, foodPos.y) = Rgb(0, 255, 0); // prepare food
    for (int i = 0; i < snakePos.size() - 1; i++) {
        logic.display().at(snakePos[i].x, snakePos[i].y) = Rgb(255, 255, 255); // prepare tail
    }
    logic.display().at(snakePos[snakePos.size() - 1].x, snakePos[snakePos.size() - 1].y) = Rgb(0, 20, 255); // prepare tail

    logic.display().show(50);
}

void logicMain() {
    logic.buttons().onPress([]() {
        std::cout << "-B UP";
        gameDirection = Up;
    },
        Up);

    logic.buttons().onPress([]() {
        std::cout << "-B DOWN";
        gameDirection = Down;
    },
        Down);

    logic.buttons().onPress([]() {
        std::cout << "-B LEFT";
        gameDirection = Left;
    },
        Left);

    logic.buttons().onPress([]() {
        std::cout << "-B RIGHT";
        gameDirection = Right;
    },
        Right);

    actualPos.x = 5;
    actualPos.y = 5;
    snakePos.push_back(actualPos);
    generateFeed();

    while (true) {
        moveGamePos();
        if (isDead()) {
            logic.display().drawRectangleFilled(0, 0, 10, 10, Rgb(255, 0, 0));
            logic.display().show(100);
            delay(500);
            logic.display().clear();
            logic.display().show(100);
            esp_restart();
        }
        showGamePos();
        std::cout << "GamePos[" << actualPos.x << ", " << actualPos.y << "]-" << std::endl;
        delay(400);
    }
}