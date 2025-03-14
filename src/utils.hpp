#pragma once

#include <cstdint>
#include <iostream>
#include <GL/glew.h>
#include "sharedData/constants.hpp"
#include "sharedData/DynamicData.hpp"

static float yAsPercentOfMaxY(const int y)
{
    return static_cast<float>(y) / static_cast<float>(Y_DISPLAY_RESOLUTION);
}

static int scaleAdcValueToY(const DynamicData &dynamicData, const uint16_t adc_value)
{
    const float current_vertical_display_resolution{
        dynamicData.verticalBoundsData.verticalUpperBound() -
        dynamicData.verticalBoundsData.verticalLowerBound()};
    const float factor{INPUT_SIGNAL_RESOLUTION /
                       current_vertical_display_resolution};
    float scaled_adc_value{
        (adc_value - dynamicData.verticalBoundsData.verticalLowerBound()) * factor};

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
        dynamicData.verticalBoundsData.verticalUpperBound_mV() -
        dynamicData.verticalBoundsData.verticalLowerBound_mV()};

    return (yAsPercentOfMaxY(y) * current_vertical_display_resolution) +
           dynamicData.verticalBoundsData.verticalLowerBound_mV();
}

static uint16_t scaleYToAdcWithinBounds(const DynamicData &dynamicData, const int y)
{
    const float current_vertical_display_resolution{
        dynamicData.verticalBoundsData.verticalUpperBound() -
        dynamicData.verticalBoundsData.verticalLowerBound()};

    return dynamicData.verticalBoundsData.verticalLowerBound() + yAsPercentOfMaxY(y) * current_vertical_display_resolution;
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

static void drawRectangle(const double x1, const double x2,
                               const double y1, const double y2, const double r,
                               const double g, const double b, const double a)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
    glVertex2f(marginCorrected(x1), marginCorrected(y1));
    glVertex2f(marginCorrected(x2), marginCorrected(y1));
    glVertex2f(marginCorrected(x2), marginCorrected(y2));
    glVertex2f(marginCorrected(x1), marginCorrected(y2));
    glEnd();

    glDisable(GL_BLEND);
}
