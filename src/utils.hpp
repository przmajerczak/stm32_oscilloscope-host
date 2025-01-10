#pragma once

#include <cstdint>

#include "settingsWindow/SettingsWindow.hpp"
#include "constants.hpp"

static float yAsPercentOfMaxY(const int y)
{
    return static_cast<float>(y) / static_cast<float>(Y_DISPLAY_RESOLUTION);
}

static int scaleAdcValueToY(const uint16_t adc_value)
{
    const float current_vertical_display_resolution{
        SettingsWindow::getVerticalUpperBoundValue() -
        SettingsWindow::getVerticalLowerBoundValue()};
    const float factor{INPUT_SIGNAL_RESOLUTION /
                       current_vertical_display_resolution};
    float scaled_adc_value{
        (adc_value - SettingsWindow::getVerticalLowerBoundValue()) * factor};

    // TODO: remove flat lines at the bounds
    if (scaled_adc_value > INPUT_SIGNAL_MAX)
    {
        scaled_adc_value = INPUT_SIGNAL_MAX;
    }
    else if (scaled_adc_value < INPUT_SIGNAL_MIN)
    {
        scaled_adc_value = INPUT_SIGNAL_MIN;
    }

    const int y{(scaled_adc_value / static_cast<float>(INPUT_SIGNAL_RESOLUTION)) *
                static_cast<float>(Y_DISPLAY_RESOLUTION)};
    return y;
}

static int scaleYToVoltage_mV(const int y)
{
    const float current_vertical_display_resolution{
        SettingsWindow::getVerticalUpperBoundValue_mV() -
        SettingsWindow::getVerticalLowerBoundValue_mV()};

    return (yAsPercentOfMaxY(y) * current_vertical_display_resolution) +
           SettingsWindow::getVerticalLowerBoundValue_mV();
}

static uint16_t scaleYToAdcWithinBounds(const int y)
{
    const float current_vertical_display_resolution{
        SettingsWindow::getVerticalUpperBoundValue() -
        SettingsWindow::getVerticalLowerBoundValue()};

    return SettingsWindow::getVerticalLowerBoundValue() + yAsPercentOfMaxY(y) * current_vertical_display_resolution;
}

static float scaleAdcTo_mV(const uint16_t adc_value)
{
    return static_cast<float>(
        (ABSOULTE_VERTICAL_RESOLUTION_mV * static_cast<float>(adc_value) +
         MIN_VOLTAGE_mV) /
        static_cast<float>(INPUT_SIGNAL_MAX));
}

template <typename T>
static inline T marginCorrected(const T &value)
{
    return value + DISPLAY_MARGIN_WIDTH;
}
