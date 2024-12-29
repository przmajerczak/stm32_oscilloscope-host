#pragma once

#include "constants.hpp"
#include "types.hpp"
#include <vector>

class DataAnalyzer
{
public:
    DataAnalyzer()
    {
        rawRetrievedValues.resize(BUFFER_SIZE);
    }

    void handleData(const std::vector<uint16_t> current_values);

private:
    bool triggerCondition();

    RawValuesContainer rawRetrievedValues;
};
