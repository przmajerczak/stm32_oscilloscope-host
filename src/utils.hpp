#pragma once

#include <cstdint>
#include <iostream>

#include "constants.hpp"

static int scaleAdcValueToY(const uint16_t adc_value,
                            const int lower_display_bound = INPUT_SIGNAL_MIN,
                            const int upper_display_bound = INPUT_SIGNAL_MAX)
{
    const float current_vertical_display_resolution{upper_display_bound - lower_display_bound};
    const float factor{INPUT_SIGNAL_RESOLUTION / current_vertical_display_resolution};
    float scaled_adc_value{(adc_value - lower_display_bound) * factor};

    // TODO: remove flat lines at the bounds
    if (scaled_adc_value > INPUT_SIGNAL_MAX)
    {
        scaled_adc_value = INPUT_SIGNAL_MAX;
    }
    else if (scaled_adc_value < INPUT_SIGNAL_MIN)
    {
        scaled_adc_value = INPUT_SIGNAL_MIN;
    }

    const int y{(scaled_adc_value / static_cast<float>(INPUT_SIGNAL_RESOLUTION)) * static_cast<float>(Y_DISPLAY_RESOLUTION)};
    return y;
}

static int scaleYToVoltage_mV(const int y)
{
    return static_cast<int>(MAX_VOLTAGE_mV * static_cast<float>(y) / static_cast<float>(Y_DISPLAY_RESOLUTION));
}

template <typename T>
static inline T marginCorrected(const T &value)
{
    return value + DISPLAY_MARGIN_WIDTH;
}

static void displayContainer(const std::string label, const auto &values)
{
    std::cout << label;
    for (const auto elem : values)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}
