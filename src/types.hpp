#pragma once

#include <list>

using RawValuesContainer = std::list<int>;


enum class ThresholdTrigger
{
    RISING_EDGE,
    FALLING_EDGE
};
