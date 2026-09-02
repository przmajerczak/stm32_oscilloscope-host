#pragma once

#include <array>
#include <cstdint>
#include <list>
#include <vector>
#include "sharedData/constants.hpp"

using AdcValue = uint16_t;

using EncodedAdcValues = std::list<uint8_t>;
using AdcValuesArray = std::array<AdcValue, SAMPLES_PER_TRANSMISSION>; // czy to nadal będzie używane?
using AdcValuesVector = std::vector<AdcValue>;
using MillivoltsArray = std::array<float, SAMPLES_PER_TRANSMISSION>;
using MillivoltsVector = std::vector<float>;
using TriggersIndexes = std::vector<std::size_t>;
using ChannelId = std::size_t;

enum class ThresholdTrigger
{
    RISING_EDGE,
    FALLING_EDGE
};

enum class DualChannelMode
{
    ON,
    OFF,
    INVALID
};

struct EncodedAdcData
{
    EncodedAdcValues values;
    ChannelId channelId;
    DualChannelMode mode;
};
