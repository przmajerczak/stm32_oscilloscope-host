#pragma once

#include <cstdint>
#include <iostream>

#include "constants.hpp"

static uint16_t scaleRawValueToDisplayHeight(const uint16_t value)
{
    return (static_cast<double>(value) / INPUT_SIGNAL_RESOLUTION) * Y_DISPLAY_RESOLUTION;
}

template <typename T>
static inline T marginCorrected(const T &value)
{
    return value + DISPLAY_MARGIN_WIDTH;
}

static void displayContainer(const std::string label, const auto &values)
{
    std::cout << label;
    for (const auto elem : values)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}
