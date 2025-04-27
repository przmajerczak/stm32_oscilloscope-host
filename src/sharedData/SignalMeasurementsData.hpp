#pragma once

#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"

struct SignalMeasurementsData
{
    AdcValue min_value{INVALID_VALUE};
    AdcValue max_value{INVALID_VALUE};
    AdcValue average_value{INVALID_VALUE};
    AdcValue amplitude{INVALID_VALUE};

    double frequency_Hz{-1.0};

    void reset()
    {
        min_value = INVALID_VALUE;
        max_value = INVALID_VALUE;
        average_value = INVALID_VALUE;
        amplitude = INVALID_VALUE;
        frequency_Hz = -1.0;
    }
};
