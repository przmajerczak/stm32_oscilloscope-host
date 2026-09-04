#pragma once

#include <array>
#include <cstdint>

#include "AdcDataDecoder.hpp"
#include "DeviceConnectionManager.hpp"
#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"

class DynamicData;

class DataRetriever
{
public:
    DataRetriever(const bool test_mode) : device{test_mode} {}
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

    DeviceConnectionManager device;
    AdcDataDecoder decoder;
    int deviceFileDescriptor;
    std::array<MillivoltsArray, NUMBER_OF_CHANNELS> retrieved_values_mV;
};
