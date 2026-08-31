#pragma once

#include "sharedData/constants.hpp"
#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <unistd.h>

class DataRetriever
{
public:
    ~DataRetriever() { close(deviceFileDescriptor); }

    void runContinuousDataRetrieve(DynamicData &dynamicData);
    AdcValuesArray getCopyOfRetrievedAdcValues(const ChannelId chId);

private:
    void establishConnection();
    std::string determineDeviceFilepath();
    void singleDataRetrieve(DynamicData &dynamicData);
    EncodedAdcData retrieveData(DynamicData &dynamicData);
    bool detectEndSequence(const uint8_t second_last_byte,
                           const uint8_t last_byte) const;
    DualChannelMode determineChannelMode(const uint8_t last_byte) const;
    ChannelId determineChannelId(const uint8_t second_last_byte) const;
    AdcValuesArray decodeAdcValues(const EncodedAdcValues &encoded_values);
    bool configureTty(const int deviceFileDescriptor);
    double calculateFrameDuration_ns(EncodedAdcValues &undecodedRetrievedData);
    uint32_t pullFrameDurationFromUndecodedRetrievedData(
        EncodedAdcValues &undecodedRetrievedData);

    int deviceFileDescriptor;
    std::array<AdcValuesArray, NUMBER_OF_CHANNELS> retrieved_adc_values;
};
