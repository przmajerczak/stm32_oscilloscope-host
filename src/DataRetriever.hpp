#pragma once

#include "DataAnalyzer.hpp"
#include <unistd.h>
#include <fcntl.h>
#include "types.hpp"

class DataRetriever
{
public:
    ~DataRetriever()
    {
        close(deviceFileDescriptor);
    }

    void runContinuousDataRetrieve();

private:
    void singleDataRetrieve();
    EncodedAdcValues retrieveData();
    AdcValues decodeAdcValues(const EncodedAdcValues& encoded_values);
    bool configureTty(const int deviceFileDescriptor);
    uint32_t pullMeasurementPeriodFromUndecodedRetrievedData(EncodedAdcValues &undecodedRetrievedData);

    DataAnalyzer dataAnalyzer;
    int deviceFileDescriptor;
};
