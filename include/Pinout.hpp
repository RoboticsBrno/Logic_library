#pragma once

#include <array>
#include <driver/gpio.h>

enum ButtonID {
    Enter = 0,
    Up,
    Down,
    Left,
    Right,
    LeftUp,
    LeftDown,
    RightUp,
    RightDown,
    MaxID,
};

namespace Pins {
namespace Corners {
static constexpr gpio_num_t LeftUp = GPIO_NUM_18;
static constexpr gpio_num_t LeftDown = GPIO_NUM_4;
static constexpr gpio_num_t RightUp = GPIO_NUM_19;
static constexpr gpio_num_t RightDown = GPIO_NUM_25;
} // namespace Buttons

namespace Arrows {
static constexpr gpio_num_t Up = GPIO_NUM_14;
static constexpr gpio_num_t Down = GPIO_NUM_26;
static constexpr gpio_num_t Left = GPIO_NUM_32;
static constexpr gpio_num_t Right = GPIO_NUM_13;
static constexpr gpio_num_t Enter = GPIO_NUM_17;
} // namespace Arrows

static constexpr gpio_num_t Display = GPIO_NUM_23;
static constexpr gpio_num_t Status = GPIO_NUM_21;

static constexpr gpio_num_t Buzzer = GPIO_NUM_27;

static constexpr gpio_num_t LedPower = GPIO_NUM_16;

static constexpr std::array<gpio_num_t, MaxID> Buttons = {
    Pins::Arrows::Enter,
    Pins::Arrows::Up,
    Pins::Arrows::Down,
    Pins::Arrows::Left,
    Pins::Arrows::Right,
    Pins::Corners::LeftUp,
    Pins::Corners::LeftDown,
    Pins::Corners::RightUp,
    Pins::Corners::RightDown,
};
} // namespace Pins