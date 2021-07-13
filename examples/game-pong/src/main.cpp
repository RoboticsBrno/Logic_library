#include "Logic.hpp"
#include <cmath>
#include <iostream>

static const int DISPLAY_INTENSITY = 20;

static const float DEFAULT_BALL_SPEED = 0.4;

static const Rgb COLOR_BALL(0, 255, 0);
static const Rgb COLOR_TOP(255, 0, 0);
static const Rgb COLOR_BOTTOM(0, 0, 255);
static const Rgb COLOR_SCORE(80, 80, 20);

static Rectangle gPaddleTop(3, 0, 4, 1);
static Rectangle gPaddleBottom(3, 9, 4, 1);

static float gBallX = 2;
static float gBallY = 0;
static float gBallSpeedX;
static float gBallSpeedY;
static int gScoreTop = 0;
static int gScoreBottom = 0;
static bool gAiDisabled = false;
static bool gPaused = false;

static void launchBall(bool toBottom) {
    // Náhodný úhel od 0 do 90 stupnu v radianech
    float angle = (float(random(0, 9000)) / 1000.f) * (M_PI / 180.f);
    if (!toBottom) {
        // Otoceni o 180 stupnu dolu + 45 aby byl 90 stupnovy vyrez uprostred
        angle += M_PI + M_PI / 2;
    } else {
        angle += M_PI / 2;
    }

    gBallSpeedX = cos(angle) * DEFAULT_BALL_SPEED;
    gBallSpeedY = sin(angle) * DEFAULT_BALL_SPEED;

    gBallX = 4 + toBottom;
    if (toBottom) {
        gBallY = 2;
    } else {
        gBallY = 8;
    }
}

static void handleVictory() {
    const auto& clr = gScoreTop >= 10 ? COLOR_TOP : COLOR_BOTTOM;
    for (int i = 0; i < 3; ++i) {
        display.fill(clr);
        display.show(DISPLAY_INTENSITY);
        waitSeconds(0.3);
        display.clear();
        display.show(DISPLAY_INTENSITY);
        waitSeconds(0.3);
    }

    gScoreTop = gScoreBottom = 0;
}

static void handleButtons() {
    const auto pressedBtns = buttons.readAll();
    if (pressedBtns[LeftDown] && gPaddleBottom.x > 0) {
        --gPaddleBottom.x;
    } else if (pressedBtns[RightDown] && gPaddleBottom.x < display.width() - gPaddleBottom.w) {
        ++gPaddleBottom.x;
    }

    if (pressedBtns[LeftUp] && gPaddleTop.x > 0) {
        gAiDisabled = true;
        --gPaddleTop.x;
    } else if (pressedBtns[RightUp] && gPaddleTop.x < display.width() - gPaddleTop.w) {
        gAiDisabled = true;
        ++gPaddleTop.x;
    }
}

static void handleAi() {
    auto paddleMid = gPaddleTop.x + float(gPaddleTop.w) / 2;
    if (abs(paddleMid - gBallX) > 0.5f) {
        if (gBallX > paddleMid && gPaddleTop.x + gPaddleTop.w < display.width()) {
            ++gPaddleTop.x;
        } else if (gBallX < paddleMid && gPaddleTop.x > 0) {
            --gPaddleTop.x;
        }
    }
}

static void handleBallCollision(int roundedX, int roundedY) {
    if ((gBallX <= 0 && gBallSpeedX < 0) || (gBallX >= display.width() - 1 && gBallSpeedX > 0)) {
        gBallSpeedX *= -1;
    }

    if ((gBallY <= 0 && gBallSpeedY < 0) || (gBallY >= display.height() - 1 && gBallSpeedY > 0)) {
        const auto& paddle = gBallY <= 0 ? gPaddleTop : gPaddleBottom;

        const float paddlePos = float(roundedX - paddle.x) / paddle.w;
        if (paddlePos < 0 || paddlePos > 1) {
            if (gBallY <= 0) {
                ++gScoreBottom;
            } else {
                ++gScoreTop;
            }

            if (gScoreTop >= 10 || gScoreBottom >= 10) {
                handleVictory();
            }
            waitSeconds(0.5);
            launchBall(gBallY > 0);
            return;
        }

        float bounceAngle = (M_PI * 0.4) + (paddlePos * M_PI * 0.6);
        if (gBallY > 0) {
            bounceAngle += M_PI;
        }

        gBallSpeedX = cos(bounceAngle) * DEFAULT_BALL_SPEED;
        gBallSpeedY = sin(bounceAngle) * DEFAULT_BALL_SPEED;

        printf("%f %f %f %f\n", paddlePos, bounceAngle, gBallSpeedX, gBallSpeedY);
    }
}

void logicMain() {
    int btnDivider = 0;
    int aiDivider = 0;

    std::vector<std::pair<int, int>> posHistory;
    auto colorTrail = COLOR_BALL;
    colorTrail.stretchChannelsEvenly(80);

    buttons.onPress([]() {
        gPaused = !gPaused;
    },
        Enter);

    launchBall(true);

    while (true) {
        waitSeconds(0.03);
        if (gPaused) {
            continue;
        }

        if (++btnDivider % 4 == 0) {
            handleButtons();
        }

        if (!gAiDisabled && ++aiDivider % 12 == 0) {
            handleAi();
        }

        gBallX += gBallSpeedX;
        gBallY += gBallSpeedY;

        const int ballIntX = round(gBallX);
        const int ballIntY = round(gBallY);

        const auto posPair = std::make_pair(ballIntX, ballIntY);
        if (posHistory.empty() || posHistory.back() != posPair) {
            posHistory.push_back(posPair);
            if (posHistory.size() > 2) {
                posHistory.erase(posHistory.begin());
            }
        }

        display.clear();
        display.drawRectangleFilled(9, 0, 1, gScoreTop, COLOR_SCORE);
        display.drawRectangleFilled(0, display.height() - gScoreBottom, 1, gScoreBottom, COLOR_SCORE);
        display.drawRectangleFilled(gPaddleTop, COLOR_TOP);
        display.drawRectangleFilled(gPaddleBottom, COLOR_BOTTOM);
        for (auto& p : posHistory) {
            display.setColor(p.first, p.second, colorTrail);
        }
        display.setColor(ballIntX, ballIntY, COLOR_BALL);
        display.show(DISPLAY_INTENSITY);

        handleBallCollision(ballIntX, ballIntY);
    }
}
