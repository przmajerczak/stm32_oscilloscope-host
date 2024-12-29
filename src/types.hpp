#pragma once

#include <cstdint>
#include <vector>

using RawValuesContainer = std::vector<uint16_t>;

enum class ThresholdTrigger
{
    RISING_EDGE,
    FALLING_EDGE
};
