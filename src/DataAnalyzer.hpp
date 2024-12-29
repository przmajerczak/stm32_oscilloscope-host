#pragma once

#include "constants.hpp"
#include "types.hpp"
#include <vector>

class DataAnalyzer
{
public:
    DataAnalyzer() {}

    void handleData(const RawValuesContainer &current_values);

private:
    RawValuesContainer centerValuesOnTrigger(const RawValuesContainer &current_values);
    std::vector<std::size_t> detectTriggers(const RawValuesContainer &current_values);
    bool isTrigger(const uint16_t leftValue, const uint16_t rightValue);
};
