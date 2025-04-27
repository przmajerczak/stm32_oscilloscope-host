#pragma once

#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"

struct SignalMeasurementsData
{
    AdcValue min_value{INVALID_VALUE};
    AdcValue max_value{INVALID_VALUE};
    AdcValue average_value{INVALID_VALUE};
    AdcValue amplitude{INVALID_VALUE};

    double frequency_Hz{0.0};
};
