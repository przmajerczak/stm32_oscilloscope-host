#pragma once

#include <cstdint>

#include "settingsWindow/controls/VerticalBoundControls.hpp"
#include "constants.hpp"

static float yAsPercentOfMaxY(const int y)
{
    return static_cast<float>(y) / static_cast<float>(Y_DISPLAY_RESOLUTION);
}

static int scaleAdcValueToY(const uint16_t adc_value)
{
    const float current_vertical_display_resolution{
        VerticalBoundControls::getVerticalUpperBoundValue() -
        VerticalBoundControls::getVerticalLowerBoundValue()};
    const float factor{INPUT_SIGNAL_RESOLUTION /
                       current_vertical_display_resolution};
    float scaled_adc_value{
        (adc_value - VerticalBoundControls::getVerticalLowerBoundValue()) * factor};

    // TODO: remove flat lines at the bounds
    if (scaled_adc_value > INPUT_SIGNAL_MAX)
    {
        scaled_adc_value = INPUT_SIGNAL_MAX;
    }
    else if (scaled_adc_value < INPUT_SIGNAL_MIN)
    {
        scaled_adc_value = INPUT_SIGNAL_MIN;
    }

    const int y{static_cast<int>((scaled_adc_value / static_cast<float>(INPUT_SIGNAL_RESOLUTION)) *
                static_cast<float>(Y_DISPLAY_RESOLUTION))};
    return y;
}

static int scaleYToVoltage_mV(const int y)
{
    const float current_vertical_display_resolution{
        VerticalBoundControls::getVerticalUpperBoundValue_mV() -
        VerticalBoundControls::getVerticalLowerBoundValue_mV()};

    return (yAsPercentOfMaxY(y) * current_vertical_display_resolution) +
           VerticalBoundControls::getVerticalLowerBoundValue_mV();
}

static uint16_t scaleYToAdcWithinBounds(const int y)
{
    const float current_vertical_display_resolution{
        VerticalBoundControls::getVerticalUpperBoundValue() -
        VerticalBoundControls::getVerticalLowerBoundValue()};

    return VerticalBoundControls::getVerticalLowerBoundValue() + yAsPercentOfMaxY(y) * current_vertical_display_resolution;
}

template <typename T>
static inline T marginCorrected(const T &value)
{
    return value + DISPLAY_MARGIN_WIDTH;
}
