#pragma once

#include "sharedData/constants.hpp"

class VerticalBoundsData
{
public:
    float vertical_lower_bound{INPUT_SIGNAL_MIN};
    float vertical_upper_bound{INPUT_SIGNAL_MAX};
    float vertical_lower_bound_mV{MIN_VOLTAGE_mV};
    float vertical_upper_bound_mV{MAX_VOLTAGE_mV};

    void notifyAboutLowerBoundChange(const float new_vertical_lower_bound)
    {
        vertical_lower_bound = new_vertical_lower_bound;
        vertical_lower_bound_mV = scaleAdcTo_mV(new_vertical_lower_bound);

        // TODO: ensure step difference, instead of equaling both sliders
        if (vertical_lower_bound > vertical_upper_bound)
        {
            vertical_upper_bound = vertical_lower_bound;
        }
    }

    void notifyAboutUpperBoundChange(const float new_vertical_upper_bound)
    {
        vertical_upper_bound = new_vertical_upper_bound;
        vertical_upper_bound_mV = scaleAdcTo_mV(new_vertical_upper_bound);

        // TODO: ensure step difference, instead of equaling both sliders
        if (vertical_upper_bound < vertical_lower_bound)
        {
            vertical_lower_bound = vertical_upper_bound;
        }
    }

private:
    float scaleAdcTo_mV(const uint16_t adc_value)
    {
        return static_cast<float>(
            (ABSOULTE_VERTICAL_RESOLUTION_mV * static_cast<float>(adc_value) +
             MIN_VOLTAGE_mV) /
            static_cast<float>(INPUT_SIGNAL_MAX));
    }
};
