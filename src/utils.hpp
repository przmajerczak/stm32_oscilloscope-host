#pragma once

#include <cstdint>
#include <iostream>

#include "settingsWindow/controls/VerticalBoundControls.hpp"
#include "sharedData/constants.hpp"
#include "sharedData/DynamicData.hpp"

static float yAsPercentOfMaxY(const int y)
{
    return static_cast<float>(y) / static_cast<float>(Y_DISPLAY_RESOLUTION);
}

static int scaleAdcValueToY(const DynamicData &dynamicData, const uint16_t adc_value)
{
    const float current_vertical_display_resolution{
        dynamicData.verticalBoundsData.vertical_upper_bound -
        dynamicData.verticalBoundsData.vertical_lower_bound};
    const float factor{INPUT_SIGNAL_RESOLUTION /
                       current_vertical_display_resolution};
    float scaled_adc_value{
        (adc_value - dynamicData.verticalBoundsData.vertical_lower_bound) * factor};

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

static int scaleYToVoltage_mV(const DynamicData &dynamicData, const int y)
{
    const float current_vertical_display_resolution{
        dynamicData.verticalBoundsData.vertical_upper_bound_mV -
        dynamicData.verticalBoundsData.vertical_lower_bound_mV};

    return (yAsPercentOfMaxY(y) * current_vertical_display_resolution) +
           dynamicData.verticalBoundsData.vertical_lower_bound_mV;
}

static uint16_t scaleYToAdcWithinBounds(const DynamicData &dynamicData, const int y)
{
    const float current_vertical_display_resolution{
        dynamicData.verticalBoundsData.vertical_upper_bound -
        dynamicData.verticalBoundsData.vertical_lower_bound};

    return dynamicData.verticalBoundsData.vertical_lower_bound + yAsPercentOfMaxY(y) * current_vertical_display_resolution;
}

static std::string doubleToFixedLengthString(const double value, std::size_t len)
{
    std::string fixedLengthNumber{std::to_string(value)};

    if (fixedLengthNumber.empty())
    {
        return fixedLengthNumber;
    }
    if (fixedLengthNumber.front() == '-')
    {
        ++len;
    }

    const auto comma_pos{fixedLengthNumber.find(',')};
    if (comma_pos > len)
    {
        std::cerr << "doubleToFixedLengthString: integer part has more digits than given truncate length." << std::endl;
        return fixedLengthNumber;
    }

    if (fixedLengthNumber.at(len) == ',')
    {
        --len;
    }
    return fixedLengthNumber.substr(0, len + 1);
}

template <typename T>
static inline T marginCorrected(const T &value)
{
    return value + DISPLAY_MARGIN_WIDTH;
}
