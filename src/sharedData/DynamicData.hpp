#pragma once

#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"
#include "sharedData/VerticalBoundsData.hpp"
#include "sharedData/TimemarkersData.hpp"
#include <cstdint>

class DynamicData
{
public:
    double frequency_Hz{0.0};
    uint16_t triggerThresholdSliderValue = DEFAULT_TRIGGER_THRESHOLD;
    ThresholdTrigger thresholdTrigger{ThresholdTrigger::FALLING_EDGE};
    double trigger_horizontal_position{X_DISPLAY_RESOLUTION / 2};

    VerticalBoundsData verticalBoundsData;
    AdcValues adcValuesToDisplay{};
    double frame_duration_ns;
    double nanoseconds_per_sample;
    uint32_t horizontal_resolution_ns{DEFAULT_HORIZONTAL_RESOLUTION_NS};
    uint16_t averaging_window_size{1};

    TimemarkersData timemarkersData;
};
