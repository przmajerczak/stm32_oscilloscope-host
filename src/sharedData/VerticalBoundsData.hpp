#pragma once

#include "sharedData/constants.hpp"

class VerticalBoundsData
{
public:
    float vertical_lower_bound{INPUT_SIGNAL_MIN};
    float vertical_upper_bound{INPUT_SIGNAL_MAX};
    float vertical_lower_bound_mV{MIN_VOLTAGE_mV};
    float vertical_upper_bound_mV{MAX_VOLTAGE_mV};
};
