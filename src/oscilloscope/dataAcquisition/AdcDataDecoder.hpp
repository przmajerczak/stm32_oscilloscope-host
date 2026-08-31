#pragma once

#include "sharedData/types.hpp"

class DynamicData;

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
