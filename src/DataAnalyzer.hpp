#pragma once

#include "types.hpp"
#include "constants.hpp"

class DataAnalyzer
{
public:
    DataAnalyzer()
    {
        raw_retrieved_values.resize(BUFFER_SIZE);
    }

    void handleData(const int current_value);

private:
    bool triggerCondition(const ThresholdTrigger trigger);

    RawValuesContainer raw_retrieved_values;
};
