#pragma once

#include "sharedData/types.hpp"
#include <cstdint>

class DynamicData
{
public:
    uint32_t thresholdTriggersSinceLastFreqLabelReset = 0;
    AdcValues adcValuesToDisplay{};
};
