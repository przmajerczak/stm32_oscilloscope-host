#pragma once

#include <cstdint>

namespace
{
    uint32_t thresholdTriggersSinceLastFreqLabelReset = 0;
}

class DynamicData
{
public:
    static uint32_t &getThresholdTriggersSinceLastFreqLabelReset()
    {
        return thresholdTriggersSinceLastFreqLabelReset;
    }
};
