#pragma once

#include <cstdint>
#include <list>
#include <vector>

using AdcValue = uint16_t;

using EncodedAdcValues = std::list<uint8_t>;
using AdcValues = std::vector<AdcValue>;
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
    OFF
};

struct EncodedAdcData
{
    EncodedAdcValues values;
    ChannelId channelId;
    DualChannelMode mode;
};
