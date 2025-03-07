#pragma once

#include "sharedData/types.hpp"
#include <vector>

#include "sharedData/DynamicData.hpp"

class DataAnalyzer
{
public:
    DataAnalyzer() {}

    void handleData(const AdcValues &current_values,
                    DynamicData &dynamicData);

private:
    AdcValues centerValuesOnTrigger(const AdcValues &averagedValues,
                                    DynamicData &dynamicData);
    AdcValues averageAdcValues(const AdcValues &current_values);
    std::vector<std::size_t> detectTriggers(const DynamicData &dynamicData,
                                            const AdcValues &averagedValues);
    bool isTrigger(const DynamicData &dynamicData, const uint16_t leftValue,
                   const uint16_t rightValue);
};
