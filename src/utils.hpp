#pragma once

#include <iostream>

#include "constants.hpp"

static int scaleRawValueToScreenHeight(const int value)
{
    return (static_cast<double>(value) / INPUT_RESOLUTION) * Y_SIZE;
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
