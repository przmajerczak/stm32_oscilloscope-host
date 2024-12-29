#pragma once

#include "constants.hpp"
#include "types.hpp"
#include <vector>

class DataAnalyzer
{
public:
    DataAnalyzer()
    {
        rawRetrievedValues_leftHalf.resize(BUFFER_SIZE / 2);
        rawRetrievedValues_rightHalf.resize(BUFFER_SIZE / 2);
    }

    void handleData(const std::vector<uint16_t> current_values);

private:
    bool triggerCondition();

    RawValuesContainer rawRetrievedValues_leftHalf;
    RawValuesContainer rawRetrievedValues_rightHalf;
};
