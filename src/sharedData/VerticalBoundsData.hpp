#pragma once

#include "sharedData/constants.hpp"

class VerticalBoundsData
{
public:
    float vertical_lower_bound{INPUT_SIGNAL_MIN};
    float vertical_upper_bound{INPUT_SIGNAL_MAX};
    float vertical_lower_bound_mV{MIN_VOLTAGE_mV};
    float vertical_upper_bound_mV{MAX_VOLTAGE_mV};

    void notifyAboutLowerBoundChange(const float new_vertical_lower_bound);
    void notifyAboutUpperBoundChange(const float new_vertical_upper_bound);

private:
    float scaleAdcTo_mV(const uint16_t adc_value) const;
};
