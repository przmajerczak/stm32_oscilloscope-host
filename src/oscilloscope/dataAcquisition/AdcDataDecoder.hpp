#pragma once

#include "sharedData/types.hpp"

class DynamicData;

class AdcDataDecoder
{
public:
    MillivoltsArray decodeAdcValuesInto_mV(const EncodedAdcValues &encoded_values);
    void fillDynamicData(DynamicData &dynamicData, EncodedAdcData &encodedRetrievedData);

private:
    double calculateFrameDuration_ns(EncodedAdcValues &encodedRetrievedData);
    uint32_t pullFrameDurationFromEncodedRetrievedData(
        EncodedAdcValues &encodedRetrievedData);
    float scaleAdcTo_mV(const AdcValue adc_value) const;
};
