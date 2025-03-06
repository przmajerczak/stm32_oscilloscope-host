#include "DataAnalyzer.hpp"
#include "debug/Timemarker.hpp"
#include "display/DisplayHelper.hpp"
#include "settingsWindow/controls/TriggerControls.hpp"
#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"
#include "utils.hpp"

void DataAnalyzer::handleData(const AdcValues &current_values,
                              DynamicData &dynamicData)
{
    Timemarker tmarker{dynamicData.timemarkersData.totalDataAnalyzeDuration};

    dynamicData.adcValuesToDisplay =
        std::move(centerValuesOnTrigger(current_values, dynamicData));
}

AdcValues DataAnalyzer::centerValuesOnTrigger(const AdcValues &current_values,
                                              DynamicData &dynamicData) // TODO: change this name

{
    const auto triggersIndexes{detectTriggers(dynamicData, current_values)};

    dynamicData.thresholdTriggersWithinFrame = triggersIndexes.size();

    AdcValues valuesToDisplay;
    if (current_values.size() == 0)
    {
        return valuesToDisplay;
    }
    const double microseconds_per_sample{dynamicData.frame_duration_us / current_values.size()};
    const uint32_t samples_to_display{dynamicData.horizontal_resolution_ns / (1000 * microseconds_per_sample)};

    valuesToDisplay.resize(samples_to_display);
    if (triggersIndexes.size() == 0)
    {
        for (std::size_t idx = 0; idx < samples_to_display; ++idx)
        {
            // TODO: copy this better with STL
            if (idx < current_values.size())
            {
                valuesToDisplay.at(idx) = current_values.at(idx);
            }
            else
            {
                valuesToDisplay.at(idx) = INVALID_VALUE;
            }
        }
        return valuesToDisplay;
    }

    std::size_t selectedTrigger{triggersIndexes.at(triggersIndexes.size() / 2)};
    const int shiftCountForTriggerCenter{
        static_cast<int>(selectedTrigger - (samples_to_display / 2))};

    for (std::size_t idx = 0; idx < valuesToDisplay.size(); ++idx)
    {
        const std::size_t current_values_idx{idx + shiftCountForTriggerCenter};
        if (current_values_idx >= 0 and
            current_values_idx < current_values.size())
        {
            valuesToDisplay.at(idx) = current_values.at(current_values_idx);
        }
        else
        {
            valuesToDisplay.at(idx) = INVALID_VALUE;
        }
    }

    return valuesToDisplay;
}

std::vector<std::size_t>
DataAnalyzer::detectTriggers(const DynamicData &dynamicData, const AdcValues &current_values)
{
    std::vector<std::size_t> triggersIndexes;

    for (std::size_t idx = 0; idx < current_values.size() - 1; ++idx)
    {
        const auto currentValue{current_values.at(idx)};
        const auto nextValue{current_values.at(idx + 1)};

        if (isTrigger(dynamicData, currentValue, nextValue))
        {
            triggersIndexes.push_back(idx);
        }
    }

    return triggersIndexes;
}

bool DataAnalyzer::isTrigger(const DynamicData &dynamicData, const uint16_t leftValue,
                             const uint16_t rightValue)
{
    const uint16_t threshold{scaleYToAdcWithinBounds(dynamicData, dynamicData.triggerThresholdSliderValue)};
    const ThresholdTrigger trigger{dynamicData.thresholdTrigger};

    if (trigger == ThresholdTrigger::RISING_EDGE)
    {
        return (leftValue < threshold and rightValue >= threshold);
    }

    if (trigger == ThresholdTrigger::FALLING_EDGE)
    {
        return (leftValue >= threshold and rightValue < threshold);
    }

    return false;
}
