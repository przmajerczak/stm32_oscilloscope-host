#pragma once

#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"

struct SignalMeasurementsData
{
    AdcValue min_value{0};
    AdcValue max_value{INVALID_VALUE};

    double frequency_Hz{0.0};
};
