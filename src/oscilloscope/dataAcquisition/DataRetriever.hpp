#pragma once

#include "AdcDataDecoder.hpp"
#include "DeviceConnectionManager.hpp"
#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"
#include <array>
#include <cstdint>

class DynamicData;

class DataRetriever
{
public:
    ~DataRetriever();

    void runContinuousDataRetrieve(DynamicData &dynamicData);
    MillivoltsArray getCopyOfRetrievedValues_mV(const ChannelId chId);

private:
    void singleDataRetrieve(DynamicData &dynamicData);
    EncodedAdcData retrieveData(DynamicData &dynamicData);
    bool detectEndSequence(const uint8_t second_last_byte,
                           const uint8_t last_byte) const;
    DualChannelMode determineChannelMode(const uint8_t last_byte) const;
    ChannelId determineChannelId(const uint8_t second_last_byte) const;

    AdcDataDecoder decoder;
    DeviceConnectionManager device;
    int deviceFileDescriptor;
    std::array<MillivoltsArray, NUMBER_OF_CHANNELS> retrieved_values_mV;
};
