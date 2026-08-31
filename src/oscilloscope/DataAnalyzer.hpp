#pragma once

#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"

class DataAnalyzer
{
public:
    DataAnalyzer() {}

    AdcValuesVector prepareData(const AdcValuesArray &current_values,
                                DynamicData &dynamicData, const ChannelId channelId);

private:
    AdcValuesVector averageAdcValues(DynamicData &dynamicData,
                                     const AdcValuesArray &current_values);
    TriggersIndexes detectTriggers(DynamicData &dynamicData,
                                   const AdcValuesVector &averaged_values,
                                   const ChannelId channelId);
    bool isTrigger(const DynamicData &dynamicData, const uint16_t leftValue,
                   const uint16_t rightValue);
    double calculateFrequency(const TriggersIndexes &triggersIndexes,
                              const double nanoseconds_per_sample,
                              const double frame_duration_ns);
    void calculateMeasurements(DynamicData &dynamicData,
                               const AdcValuesVector adc_values_to_display,
                               const TriggersIndexes &triggersIndexes,
                               const ChannelId channelId);
};
