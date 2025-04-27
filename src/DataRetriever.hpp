#pragma once

#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"
#include <cstdint>
#include <string>
#include <unistd.h>

class DataRetriever
{
public:
    ~DataRetriever() { close(deviceFileDescriptor); }

    void runContinuousDataRetrieve(DynamicData &dynamicData);

private:
    void establishConnection();
    std::string determineDeviceFilepath();
    void singleDataRetrieve(DynamicData &dynamicData);
    EncodedAdcData retrieveData(DynamicData &dynamicData);
    DualChannelMode determineChannelMode(const uint8_t last_byte) const;
    ChannelId determineChannelId(const uint8_t second_last_byte) const;
    AdcValues decodeAdcValues(const EncodedAdcValues &encoded_values);
    bool configureTty(const int deviceFileDescriptor);
    double calculateFrameDuration_ns(EncodedAdcValues &undecodedRetrievedData);
    uint32_t pullFrameDurationFromUndecodedRetrievedData(
        EncodedAdcValues &undecodedRetrievedData);

    int deviceFileDescriptor;
};
