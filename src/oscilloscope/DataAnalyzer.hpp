#pragma once

#include "sharedData/types.hpp"

class DynamicData;

class DataAnalyzer
{
public:
    DataAnalyzer() {}

    MillivoltsVector prepareData(const MillivoltsArray &current_values,
                                DynamicData &dynamicData, const ChannelId channelId);

private:
    MillivoltsVector applyAveraging(DynamicData &dynamicData,
                                     const MillivoltsArray &current_values);
    TriggersIndexes detectTriggers(DynamicData &dynamicData,
                                   const MillivoltsVector &averaged_values,
                                   const ChannelId channelId);
    bool isTrigger(const DynamicData &dynamicData, const float leftValue,
                   const float rightValue);
    double calculateFrequency(const TriggersIndexes &triggersIndexes,
                              const double nanoseconds_per_sample,
                              const double frame_duration_ns);
    void calculateMeasurements(DynamicData &dynamicData,
                               const MillivoltsVector values_to_display_mV,
                               const TriggersIndexes &triggersIndexes,
                               const ChannelId channelId);
};
