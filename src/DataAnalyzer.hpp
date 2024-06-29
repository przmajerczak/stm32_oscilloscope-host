#pragma once

#include "types.hpp"
#include "constants.hpp"
#include <cstdint>

class DataAnalyzer
{
public:
    DataAnalyzer()
    {
        raw_retrieved_values.resize(BUFFER_SIZE);
    }

    void handleData(const int current_value);

private:
    bool triggerCondition(const uint16_t threshold, const ThresholdTrigger trigger);

    RawValuesContainer raw_retrieved_values;
};
