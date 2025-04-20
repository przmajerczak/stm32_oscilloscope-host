#pragma once

#include "sharedData/constants.hpp"
#include <cstdint>

class VerticalBoundsData
{
public:
    uint16_t &verticalLowerBound() { return vertical_lower_bound; }
    uint16_t &verticalUpperBound() { return vertical_upper_bound; }
    int16_t &verticalLowerBound_mV() { return vertical_lower_bound_mV; }
    int16_t &verticalUpperBound_mV() { return vertical_upper_bound_mV; }

    const uint16_t verticalLowerBound() const { return vertical_lower_bound; }
    const uint16_t verticalUpperBound() const { return vertical_upper_bound; }
    const int16_t verticalLowerBound_mV() const { return vertical_lower_bound_mV; }
    const int16_t verticalUpperBound_mV() const { return vertical_upper_bound_mV; }

    void notifyAboutLowerBoundChange(const uint16_t new_vertical_lower_bound);
    void notifyAboutUpperBoundChange(const uint16_t new_vertical_upper_bound);

private:
    uint16_t vertical_lower_bound{INPUT_SIGNAL_MIN};
    uint16_t vertical_upper_bound{INPUT_SIGNAL_MAX};
    int16_t vertical_lower_bound_mV{MIN_VOLTAGE_mV};
    int16_t vertical_upper_bound_mV{MAX_VOLTAGE_mV};

    int16_t scaleAdcTo_mV(const uint16_t adc_value) const;
};
