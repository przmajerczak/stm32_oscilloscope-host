#pragma once

#include <cstdint>
#include <iostream>

#include "constants.hpp"

static uint16_t scaleAdcValueToDisplayHeight(const uint16_t adc_value)
{
    return (static_cast<double>(adc_value) / INPUT_SIGNAL_RESOLUTION) * Y_DISPLAY_RESOLUTION;
}

static int scaleYToVoltage_mV(const uint16_t y)
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
