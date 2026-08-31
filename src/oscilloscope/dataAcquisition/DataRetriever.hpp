#pragma once

#include "DeviceConnectionManager.hpp"
#include "sharedData/constants.hpp"
#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"
#include <array>
#include <cstdint>

class DataRetriever
{
public:
    ~DataRetriever();

    void runContinuousDataRetrieve(DynamicData &dynamicData);
    AdcValuesArray getCopyOfRetrievedAdcValues(const ChannelId chId);

private:
    void singleDataRetrieve(DynamicData &dynamicData);
    EncodedAdcData retrieveData(DynamicData &dynamicData);
    bool detectEndSequence(const uint8_t second_last_byte,
                           const uint8_t last_byte) const;
    DualChannelMode determineChannelMode(const uint8_t last_byte) const;
    ChannelId determineChannelId(const uint8_t second_last_byte) const;
    AdcValuesArray decodeAdcValues(const EncodedAdcValues &encoded_values);
    double calculateFrameDuration_ns(EncodedAdcValues &undecodedRetrievedData);
    uint32_t pullFrameDurationFromUndecodedRetrievedData(
        EncodedAdcValues &undecodedRetrievedData);

    DeviceConnectionManager device;
    int deviceFileDescriptor;
    std::array<AdcValuesArray, NUMBER_OF_CHANNELS> retrieved_adc_values;
};
