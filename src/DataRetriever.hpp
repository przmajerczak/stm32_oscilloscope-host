#pragma once

#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"
#include <cstdint>
#include <unistd.h>

class DataRetriever
{
public:
    ~DataRetriever() { close(deviceFileDescriptor); }

    void runContinuousDataRetrieve(DynamicData &dynamicData);

private:
    void establishConnection();
    void singleDataRetrieve(DynamicData &dynamicData);
    EncodedAdcData retrieveData(DynamicData &dynamicData);
    AdcValues decodeAdcValues(const EncodedAdcValues &encoded_values);
    bool configureTty(const int deviceFileDescriptor);
    double calculateFrameDuration_ns(EncodedAdcValues &undecodedRetrievedData);
    uint32_t pullFrameDurationFromUndecodedRetrievedData(
        EncodedAdcValues &undecodedRetrievedData);

    int deviceFileDescriptor;
};
