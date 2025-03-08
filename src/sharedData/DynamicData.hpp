#pragma once

#include "sharedData/TimemarkersData.hpp"
#include "sharedData/VerticalBoundsData.hpp"
#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"
#include <cstdint>

class GlobalData
{
public:
    VerticalBoundsData verticalBoundsData;
    double frame_duration_ns;
    uint32_t horizontal_resolution_ns{DEFAULT_HORIZONTAL_RESOLUTION_NS};
    TimemarkersData timemarkersData;
};

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
class DynamicData
{
public:
    GlobalData globalData;
    ChannelData channelData;
};
