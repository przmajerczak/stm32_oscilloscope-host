#pragma once

#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"
#include <cstdint>

class ChannelData
{
public:
    double frequency_Hz{0.0};
    uint16_t triggerThresholdSliderValue = DEFAULT_TRIGGER_THRESHOLD;
    ThresholdTrigger thresholdTrigger{ThresholdTrigger::FALLING_EDGE};
    double trigger_horizontal_position{X_DISPLAY_RESOLUTION /
                                       (NUMBER_OF_CHANNELS + 1)};
    AdcValues adcValuesToDisplay{};
    double nanoseconds_per_sample;
    uint16_t averaging_window_size{1};
};
