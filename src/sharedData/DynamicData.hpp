#pragma once

#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"
#include "sharedData/VerticalBoundsData.hpp"
#include "sharedData/TimemarkersData.hpp"
#include <cstdint>

class DynamicData
{
public:
    uint32_t thresholdTriggersWithinFrame = 0;
    uint16_t triggerThresholdSliderValue = DEFAULT_TRIGGER_THRESHOLD;
    ThresholdTrigger thresholdTrigger{ThresholdTrigger::FALLING_EDGE};

    VerticalBoundsData verticalBoundsData;
    AdcValues adcValuesToDisplay{};
    double frame_duration_us;
    uint32_t horizontal_resolution_us{DEFAULT_HORIZONTAL_RESOLUTION_US};
    TimemarkersData timemarkersData;
};
