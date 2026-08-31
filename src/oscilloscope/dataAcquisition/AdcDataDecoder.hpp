#pragma once

#include "sharedData/types.hpp"
#include "sharedData/DynamicData.hpp"

class AdcDataDecoder
{
public:
    AdcValuesArray decodeAdcValues(const EncodedAdcValues &encoded_values);
    void fillDynamicData(DynamicData &dynamicData, EncodedAdcData &encodedRetrievedData);

private:
    double calculateFrameDuration_ns(EncodedAdcValues &encodedRetrievedData);
    uint32_t pullFrameDurationFromEncodedRetrievedData(
        EncodedAdcValues &encodedRetrievedData);
};
