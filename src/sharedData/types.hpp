#pragma once

#include <cstdint>
#include <list>
#include <vector>

using EncodedAdcValues = std::list<uint8_t>;
using AdcValues = std::vector<uint16_t>;

enum class ThresholdTrigger
{
    RISING_EDGE,
    FALLING_EDGE
};
