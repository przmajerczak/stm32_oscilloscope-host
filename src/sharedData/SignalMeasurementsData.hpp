#pragma once

#include "sharedData/constants.hpp"

struct SignalMeasurementsData
{
    float min_value_mV{INVALID_VALUE_FLOAT};
    float max_value_mV{INVALID_VALUE_FLOAT};
    float average_value_mV{INVALID_VALUE_FLOAT};
    float amplitude_mV{INVALID_VALUE_FLOAT};

    double frequency_Hz{-1.0};

    void reset()
    {
        min_value_mV = INVALID_VALUE_FLOAT;
        max_value_mV = INVALID_VALUE_FLOAT;
        average_value_mV = INVALID_VALUE_FLOAT;
        amplitude_mV = INVALID_VALUE_FLOAT;
        frequency_Hz = -1.0;
    }
};
