#pragma once

#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"
#include "sharedData/VerticalBoundsData.hpp"
#include "sharedData/TimemarkersData.hpp"
#include <array>
#include <cstdint>

class DynamicData
{
public:
    double frequency_Hz{0.0};
    uint16_t triggerThresholdSliderValue = DEFAULT_TRIGGER_THRESHOLD;
    ThresholdTrigger thresholdTrigger{ThresholdTrigger::FALLING_EDGE};
    double trigger_horizontal_position{X_DISPLAY_RESOLUTION / 2};
    std::size_t trigger_source{DEFAULT_TRIGGER_SOURCE};
    std::size_t trigger_index{0};

    VerticalBoundsData verticalBoundsData;
    std::array<AdcValues, NUMBER_OF_CHANNELS> retrieved_adc_values;
    double frame_duration_ns;
    double nanoseconds_per_sample;
    uint32_t horizontal_resolution_ns{DEFAULT_HORIZONTAL_RESOLUTION_NS};
    uint16_t averaging_window_size{1};

    TimemarkersData timemarkersData;
};
