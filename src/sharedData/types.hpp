#pragma once

#include <cstdint>
#include <list>
#include <vector>

using EncodedAdcValues = std::list<uint8_t>;
using AdcValues = std::vector<uint16_t>;
using TriggersIndexes = std::vector<std::size_t>;

enum class ThresholdTrigger
{
    RISING_EDGE,
    FALLING_EDGE
};

enum class ChannelId
{
    CHANNEL_1,
    CHANNEL_2
};

struct EncodedAdcData
{
    EncodedAdcValues values;
    ChannelId channelId;
};
