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

namespace LeftArrows {
static constexpr gpio_num_t Up = GPIO_NUM_47;
static constexpr gpio_num_t Down = GPIO_NUM_7;
static constexpr gpio_num_t Left = GPIO_NUM_1;
static constexpr gpio_num_t Right = GPIO_NUM_6;
static constexpr gpio_num_t Enter = GPIO_NUM_5;
} // namespace LeftArrows

namespace RightArrows {
static constexpr gpio_num_t Up = GPIO_NUM_8;
static constexpr gpio_num_t Down = GPIO_NUM_14;
static constexpr gpio_num_t Left = GPIO_NUM_10;
static constexpr gpio_num_t Right = GPIO_NUM_4;
static constexpr gpio_num_t Enter = GPIO_NUM_9;
} // namespace RightArrows

static constexpr gpio_num_t Display = GPIO_NUM_45;
static constexpr gpio_num_t Status = GPIO_NUM_46;

static constexpr gpio_num_t Buzzer = GPIO_NUM_27;
static constexpr bool GeneratorBuzzer = false;

static constexpr gpio_num_t LedPower = GPIO_NUM_0;
static constexpr bool InvertLedPower = true;

static constexpr std::array<gpio_num_t, MaxID> Buttons = {
    Pins::LeftArrows::Enter,
    Pins::LeftArrows::Up,
    Pins::LeftArrows::Down,
    Pins::LeftArrows::Left,
    Pins::LeftArrows::Right,
    
    Pins::RightArrows::Enter,
    Pins::RightArrows::Up,
    Pins::RightArrows::Down,
    Pins::RightArrows::Left,
    Pins::RightArrows::Right,
};
} // namespace Pins