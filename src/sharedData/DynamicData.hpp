#pragma once

#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"
#include <cstdint>

class DynamicData
{
public:
    uint32_t thresholdTriggersSinceLastFreqLabelReset = 0;
    uint16_t triggerThresholdSliderValue = DEFAULT_TRIGGER_THRESHOLD;
    ThresholdTrigger thresholdTrigger{ThresholdTrigger::FALLING_EDGE};

    AdcValues adcValuesToDisplay{};
};
