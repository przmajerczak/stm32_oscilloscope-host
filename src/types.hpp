#pragma once

#include <cstdint>
#include <list>

using RawValuesContainer = std::list<uint16_t>;

enum class ThresholdTrigger
{
    RISING_EDGE,
    FALLING_EDGE
};
