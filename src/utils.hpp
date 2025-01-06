#pragma once

#include <cstdint>
#include <iostream>

#include "constants.hpp"

static uint16_t scaleRawValueToScreenHeight(const uint16_t value)
{
    return (static_cast<double>(value) / INPUT_SIGNAL_RESOLUTION) * Y_DISPLAY_RESOLUTION;
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
