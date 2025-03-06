#pragma once

#include "DataAnalyzer.hpp"
#include "sharedData/types.hpp"
#include <unistd.h>

#include "sharedData/DynamicData.hpp"

class DataRetriever
{
public:
    ~DataRetriever() { close(deviceFileDescriptor); }

    void runContinuousDataRetrieve(DynamicData &dynamicData);

private:
    AdcValues singleDataRetrieve(DynamicData &dynamicData);
    EncodedAdcValues retrieveData(DynamicData &dynamicData);
    AdcValues decodeAdcValues(const EncodedAdcValues &encoded_values);
    bool configureTty(const int deviceFileDescriptor);
    double calculateFrameDuration_ns(EncodedAdcValues &undecodedRetrievedData);
    uint32_t pullFrameDurationFromUndecodedRetrievedData(
        EncodedAdcValues &undecodedRetrievedData);

    DataAnalyzer dataAnalyzer;
    int deviceFileDescriptor;
};
