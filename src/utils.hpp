#pragma once

#include "sharedData/DynamicData.hpp"
#include "sharedData/constants.hpp"
#include <GL/glew.h>
#include <cstdint>
#include <iostream>

static float scaleAdcValueToY(const DynamicData &dynamicData,
                            const uint16_t adc_value)
{
    const float current_vertical_display_resolution{
        dynamicData.verticalBoundsData.verticalUpperBound() -
        dynamicData.verticalBoundsData.verticalLowerBound()};
    const float factor{INPUT_SIGNAL_RESOLUTION /
                       current_vertical_display_resolution};
    float scaled_adc_value{
        (adc_value - dynamicData.verticalBoundsData.verticalLowerBound()) *
        factor};

    if (scaled_adc_value > INPUT_SIGNAL_MAX)
    {
        scaled_adc_value = INPUT_SIGNAL_MAX;
    }
    else if (scaled_adc_value < INPUT_SIGNAL_MIN)
    {
        scaled_adc_value = INPUT_SIGNAL_MIN;
    }

    const float y{
        (scaled_adc_value / static_cast<float>(INPUT_SIGNAL_RESOLUTION)) *
        static_cast<float>(Y_DISPLAY_RESOLUTION)};
    return y;
}

static int scaleYToVoltage_mV(const DynamicData &dynamicData, const float y)
{
    const float current_vertical_display_resolution{
        dynamicData.verticalBoundsData.verticalUpperBound_mV() -
        dynamicData.verticalBoundsData.verticalLowerBound_mV()};

    return ((y * current_vertical_display_resolution) /
            static_cast<float>(Y_DISPLAY_RESOLUTION)) +
           dynamicData.verticalBoundsData.verticalLowerBound_mV();
}

static uint16_t scaleYToAdcWithinBounds(const DynamicData &dynamicData,
                                        const float y)
{
    const float current_vertical_display_resolution{
        dynamicData.verticalBoundsData.verticalUpperBound() -
        dynamicData.verticalBoundsData.verticalLowerBound()};

    return ((y * current_vertical_display_resolution) /
            static_cast<float>(Y_DISPLAY_RESOLUTION)) +
           dynamicData.verticalBoundsData.verticalLowerBound();
}

static double scaleXToNanoseconds(const DynamicData &dynamicData,
                                  const double x)
{
    const double x_zero_ns{dynamicData.trigger_horizontal_position};
    const double nanoseconds_per_x{dynamicData.horizontal_resolution_ns /
                                   static_cast<double>(X_DISPLAY_RESOLUTION)};

    return (x - x_zero_ns) * nanoseconds_per_x;
}

static std::string doubleToFixedLengthString(const double value,
                                             std::size_t len)
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
        std::cerr << "doubleToFixedLengthString: integer part has more digits than "
                     "given truncate length."
                  << std::endl;
        return fixedLengthNumber;
    }

    if (fixedLengthNumber.at(len) == ',')
    {
        --len;
    }
    return fixedLengthNumber.substr(0, len + 1);
}

static std::pair<std::string, const char *>
nanosecondsToNormalizedTimestampAndUnit(const DynamicData &dynamicData,
                                        const double nanoseconds)
{
    double time_multiplier{1.0f};
    const char *unit_label{"ns"};

    if (dynamicData.horizontal_resolution_ns >= 1000000000)
    {
        time_multiplier = 0.000000001;
        unit_label = "s";
    }
    else if (dynamicData.horizontal_resolution_ns >= 1000000)
    {
        time_multiplier = 0.000001;
        unit_label = "ms";
    }
    else if (dynamicData.horizontal_resolution_ns >= 10000)
    {
        time_multiplier = 0.001;
        unit_label = "us";
    }

    return {doubleToFixedLengthString(nanoseconds * time_multiplier, 4).c_str(),
            unit_label};
}

template <typename T>
static inline T marginCorrected(const T &value)
{
    return value + DISPLAY_MARGIN_WIDTH;
}

static void drawRectangle(const double x1, const double x2, const double y1,
                          const double y2, const float *gl_color)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4fv(gl_color);
    glBegin(GL_QUADS);
    glVertex2f(marginCorrected(x1), marginCorrected(y1));
    glVertex2f(marginCorrected(x2), marginCorrected(y1));
    glVertex2f(marginCorrected(x2), marginCorrected(y2));
    glVertex2f(marginCorrected(x1), marginCorrected(y2));
    glEnd();

    glDisable(GL_BLEND);
}

static void drawOutline(const double x1, const double x2, const double y1,
                        const double y2, const float *gl_color,
                        const double boldness = BOLD)
{
    glLineWidth(boldness);
    glColor4fv(gl_color);

    glBegin(GL_LINE_LOOP);

    glVertex2f(marginCorrected(x1), marginCorrected(y1));
    glVertex2f(marginCorrected(x1), marginCorrected(y2));
    glVertex2f(marginCorrected(x2), marginCorrected(y2));
    glVertex2f(marginCorrected(x2), marginCorrected(y1));

    glEnd();
}
