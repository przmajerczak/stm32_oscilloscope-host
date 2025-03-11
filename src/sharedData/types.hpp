#pragma once

#include <cstdint>
#include <list>
#include <vector>

using EncodedAdcValues = std::list<uint8_t>;
using AdcValues = std::vector<uint16_t>;
using TriggersIndexes = std::vector<std::size_t>;
using ChannelId = std::size_t;

enum class ThresholdTrigger
{
    RISING_EDGE,
    FALLING_EDGE
};

struct EncodedAdcData
{
    EncodedAdcValues values;
    ChannelId channelId;
};
