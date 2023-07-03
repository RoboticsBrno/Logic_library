#pragma once

#include <array>
#include <driver/gpio.h>

enum ButtonID {
    L_Enter = 0,
    L_Up,
    L_Down,
    L_Left,
    L_Right,
    R_Enter,
    R_Up,
    R_Down,
    R_Left,
    R_Right,
    MaxID,
};

namespace Pins {

namespace Arrows {
static constexpr gpio_num_t L_Up = GPIO_NUM_47;
static constexpr gpio_num_t L_Down = GPIO_NUM_7;
static constexpr gpio_num_t L_Left = GPIO_NUM_1;
static constexpr gpio_num_t L_Right = GPIO_NUM_6;
static constexpr gpio_num_t L_Enter = GPIO_NUM_5;
static constexpr gpio_num_t R_Up = GPIO_NUM_8;
static constexpr gpio_num_t R_Down = GPIO_NUM_14;
static constexpr gpio_num_t R_Left = GPIO_NUM_10;
static constexpr gpio_num_t R_Right = GPIO_NUM_4;
static constexpr gpio_num_t R_Enter = GPIO_NUM_9;
} // namespace Arrows

static constexpr gpio_num_t Display = GPIO_NUM_45;
static constexpr gpio_num_t Status = GPIO_NUM_46;

static constexpr gpio_num_t Buzzer = GPIO_NUM_27;
static constexpr bool GeneratorBuzzer = false;

static constexpr gpio_num_t LedPower = GPIO_NUM_0;
static constexpr bool InvertLedPower = true;

static constexpr std::array<gpio_num_t, MaxID> Buttons = {
    Pins::Arrows::L_Enter,
    Pins::Arrows::L_Up,
    Pins::Arrows::L_Down,
    Pins::Arrows::L_Left,
    Pins::Arrows::L_Right,
    Pins::Arrows::R_Enter,
    Pins::Arrows::R_Up,
    Pins::Arrows::R_Down,
    Pins::Arrows::R_Left,
    Pins::Arrows::R_Right,
};
} // namespace Pins