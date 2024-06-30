#pragma once

#include "constants.hpp"
#include "types.hpp"

class DataAnalyzer
{
public:
    DataAnalyzer()
    {
        rawRetrievedValues_leftHalf.resize(BUFFER_SIZE / 2);
        rawRetrievedValues_rightHalf.resize(BUFFER_SIZE / 2);
    }

    void handleData(const int current_value);

private:
    bool triggerCondition();

    RawValuesContainer rawRetrievedValues_leftHalf;
    RawValuesContainer rawRetrievedValues_rightHalf;
};
