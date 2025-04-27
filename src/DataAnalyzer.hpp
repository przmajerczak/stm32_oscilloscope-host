#pragma once

#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"

class DataAnalyzer
{
public:
    DataAnalyzer() {}

    AdcValues prepareData(const AdcValues &current_values,
                          DynamicData &dynamicData, const ChannelId channelId);

private:
    AdcValues centerValuesOnTrigger(const AdcValues &averaged_values,
                                    DynamicData &dynamicData);
    AdcValues averageAdcValues(DynamicData &dynamicData,
                               const AdcValues &current_values);
    TriggersIndexes detectTriggers(DynamicData &dynamicData,
                                   const AdcValues &averaged_values,
                                   const ChannelId channelId);
    bool isTrigger(const DynamicData &dynamicData, const uint16_t leftValue,
                   const uint16_t rightValue);
    double calculateFrequency(const TriggersIndexes &triggersIndexes,
                              const double nanoseconds_per_sample,
                              const double frame_duration_ns);
    void calculateMeasurements(DynamicData &dynamicData,
                               const AdcValues adc_values_to_display,
                               const TriggersIndexes &triggersIndexes,
                               const ChannelId channelId);
};
