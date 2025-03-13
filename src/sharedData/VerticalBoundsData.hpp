#pragma once

#include "sharedData/constants.hpp"

class VerticalBoundsData
{
public:
    float &verticalLowerBound() { return vertical_lower_bound; }
    float &verticalUpperBound() { return vertical_upper_bound; }
    float &verticalLowerBound_mV() { return vertical_lower_bound_mV; }
    float &verticalUpperBound_mV() { return vertical_upper_bound_mV; }

    const float verticalLowerBound() const { return vertical_lower_bound; }
    const float verticalUpperBound() const { return vertical_upper_bound; }
    const float verticalLowerBound_mV() const { return vertical_lower_bound_mV; }
    const float verticalUpperBound_mV() const { return vertical_upper_bound_mV; }

    void notifyAboutLowerBoundChange(const float new_vertical_lower_bound);
    void notifyAboutUpperBoundChange(const float new_vertical_upper_bound);

private:
    float vertical_lower_bound{INPUT_SIGNAL_MIN};
    float vertical_upper_bound{INPUT_SIGNAL_MAX};
    float vertical_lower_bound_mV{MIN_VOLTAGE_mV};
    float vertical_upper_bound_mV{MAX_VOLTAGE_mV};

    float scaleAdcTo_mV(const uint16_t adc_value) const;
};
