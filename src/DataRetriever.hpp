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
    void singleDataRetrieve(DynamicData &dynamicData);
    EncodedAdcValues retrieveData();
    AdcValues decodeAdcValues(const EncodedAdcValues &encoded_values);
    bool configureTty(const int deviceFileDescriptor);
    double calculateFrameDuration_us(EncodedAdcValues &undecodedRetrievedData);
    uint32_t pullFrameDurationFromUndecodedRetrievedData(
        EncodedAdcValues &undecodedRetrievedData);

    DataAnalyzer dataAnalyzer;
    int deviceFileDescriptor;
};
