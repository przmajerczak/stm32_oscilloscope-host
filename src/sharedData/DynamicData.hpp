#pragma once

#include "sharedData/HorizontalMeasurementsData.hpp"
#include "sharedData/SignalMeasurementsData.hpp"
#include "sharedData/TimemarkersData.hpp"
#include "sharedData/VerticalBoundsData.hpp"
#include "sharedData/VerticalMeasurementsData.hpp"
#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"
#include <array>
#include <cstdint>

class DynamicData
{
public:
    uint16_t triggerThresholdSliderValue = DEFAULT_TRIGGER_THRESHOLD;
    ThresholdTrigger thresholdTrigger{ThresholdTrigger::RISING_EDGE};
    double trigger_horizontal_position{X_DISPLAY_RESOLUTION / 2};
    std::size_t trigger_source{DEFAULT_TRIGGER_SOURCE};
    std::size_t trigger_index{0};

    VerticalBoundsData verticalBoundsData;
    std::array<bool, NUMBER_OF_CHANNELS> active_channels;
    std::array<AdcValues, NUMBER_OF_CHANNELS> retrieved_adc_values;
    std::array<SignalMeasurementsData, NUMBER_OF_CHANNELS> signalMeasurementsData;
    double frame_duration_ns;
    double nanoseconds_per_sample;
    uint32_t horizontal_resolution_ns{DEFAULT_HORIZONTAL_RESOLUTION_NS};
    uint16_t averaging_window_size{1};
    VerticalMeasurementsData verticalMeasurementsData;
    HorizontalMeasurementsData horizontalMeasurementsData;
    bool new_data_available{true};
    bool display_configuration_changed{false};

    TimemarkersData timemarkersData;
};
