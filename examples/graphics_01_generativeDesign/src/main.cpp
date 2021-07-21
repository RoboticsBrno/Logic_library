#include "Logic.hpp"
#include <iostream>
#include <cmath>
#include <utility>

float gammaNormalize(float x) {
    float y = pow(x, 2.8);
    return std::min(y, 1.0f);
}

void showSinCosPattern(int tick) {
    float phase = tick / 3.14 / 5;
    for (int x = 0; x != 10; x++) {
        for (int y = 0; y != 10; y++) {
            float redIntensity = (1 + sin(x / 3.14 + phase)) * (1 + sin(y / 3.14 + phase)) / 4.0;
            float greenIntensity = (1 + sin(-x + phase)) * (1 + sin(-y * 2 + phase)) / 4.0;

            display.setColor(x, y, Rgb(gammaNormalize(redIntensity) * 255, gammaNormalize(greenIntensity) * 255, 0));
        }
    }
}

void showSinCosPatternDouble(int tick) {
    float phase = tick / 3.14 / 5;
    for (int x = 0; x != 10; x++) {
        for (int y = 0; y != 10; y++) {
            float redIntensity = (1 + sin(x / 3.14 + phase)) * (1 + sin(y / 3.14 + phase)) / 4.0;
            float greenIntensity = (1 + sin(-x / 3.14 + phase)) * (1 + sin(-y / 3.14 + phase)) / 4.0;

            display.setColor(x, y, Rgb(gammaNormalize(redIntensity) * 255, gammaNormalize(greenIntensity) * 255, 0));
        }
    }
}

void showSinCosPatternTriple(int tick) {
    float phase = tick / 3.14 / 5;
    for (int x = 0; x != 10; x++) {
        for (int y = 0; y != 10; y++) {
            float redIntensity = (1 + sin(x / 3.14 + phase)) * (1 + sin(y / 3.14 + phase)) / 4.0;
            float blueIntensity = (1 + sin(-x / 3.14 + phase)) * (1 + sin(-y / 3.14 + phase)) / 4.0;
            float greenIntensity = (1 + sin(x / 3.14 + phase)) * (1 + sin(-y / 3.14 + phase)) / 4.0;

            display.setColor(x, y, Rgb(gammaNormalize(redIntensity) * 255, gammaNormalize(greenIntensity) * 255, gammaNormalize(blueIntensity) * 255));
        }
    }
}

void showSinCosPatternRowAlt(int tick) {
    float phase = tick / 3.14 / 5;
    for (int x = 0; x != 10; x++) {
        for (int y = 0; y != 10; y++) {
            float intensity = (1 + sin(x *  3.14 + phase)) * (1 + sin(y / 3.14 + phase)) / 4.0;
            display.setColor(x, y, Rgb(gammaNormalize(intensity) * 255, 0, 0));
        }
    }
}

template < typename F >
void runFor(int delayMs, F f) {
    for (int tick = 0; tick < delayMs / 10; tick++) {
        f(tick);
        display.show();
        waitSeconds(0.01);
    }
}

void logicMain() {
    while (true) {
        runFor(4000, showSinCosPattern);
        runFor(4000, showSinCosPatternDouble);
        runFor(4000, showSinCosPatternTriple);
        runFor(4000, showSinCosPatternRowAlt);
    }
}