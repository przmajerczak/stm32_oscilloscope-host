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
    bool triggerCondition();
};
