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

AdcValues DataAnalyzer::centerValuesOnTrigger(
    const AdcValues &current_values,
    DynamicData &dynamicData) // TODO: change this name

{
    const auto averagedValues{averageAdcValues(current_values)};
    const auto triggersIndexes{detectTriggers(dynamicData, averagedValues)};

    dynamicData.thresholdTriggersWithinFrame = triggersIndexes.size();

    AdcValues valuesToDisplay;
    if (averagedValues.size() == 0)
    {
        return valuesToDisplay;
    }
    const double nanoseconds_per_sample{dynamicData.frame_duration_ns /
                                        averagedValues.size()};
    const uint32_t samples_to_display{dynamicData.horizontal_resolution_ns /
                                      nanoseconds_per_sample};

    dynamicData.nanoseconds_per_sample = nanoseconds_per_sample;

    valuesToDisplay.resize(samples_to_display);
    if (triggersIndexes.size() == 0)
    {
        for (std::size_t idx = 0; idx < samples_to_display; ++idx)
        {
            // TODO: copy this better with STL
            if (idx < averagedValues.size())
            {
                valuesToDisplay.at(idx) = averagedValues.at(idx);
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
        const std::size_t averagedValues_idx{idx + shiftCountForTriggerCenter};
        if (averagedValues_idx >= 0 and
            averagedValues_idx < averagedValues.size())
        {
            valuesToDisplay.at(idx) = averagedValues.at(averagedValues_idx);
        }
        else
        {
            valuesToDisplay.at(idx) = INVALID_VALUE;
        }
    }

    return valuesToDisplay;
}

AdcValues DataAnalyzer::averageAdcValues(const AdcValues &current_values)
{
    constexpr uint16_t MOVING_AVERAGE_WINDOW_SIZE{1};

    if (MOVING_AVERAGE_WINDOW_SIZE < 2)
    {
        return std::move(current_values);
    }

    if (MOVING_AVERAGE_WINDOW_SIZE > current_values.size())
    {
        std::cerr << "Moving average window size exceeds number of samples per "
                     "frame - averaging not possible."
                  << std::endl;
        return std::move(current_values);
    }

    AdcValues averaged_values;
    averaged_values.resize(current_values.size() - MOVING_AVERAGE_WINDOW_SIZE -
                           1);

    auto moving_average_window_front{current_values.begin()};
    auto moving_average_window_back{
        std::next(moving_average_window_front, MOVING_AVERAGE_WINDOW_SIZE)};

    for (auto &averaged_value : averaged_values)
    {
        averaged_value = static_cast<AdcValues::value_type>(
            static_cast<double>(std::accumulate(moving_average_window_front,
                                                moving_average_window_back, 0)) /
            static_cast<double>(MOVING_AVERAGE_WINDOW_SIZE));

        moving_average_window_front++;
        moving_average_window_back++;
    }

    return std::move(averaged_values);
}

std::vector<std::size_t>
DataAnalyzer::detectTriggers(const DynamicData &dynamicData,
                             const AdcValues &averagedValues)
{
    std::vector<std::size_t> triggersIndexes;

    for (std::size_t idx = 0; idx < averagedValues.size() - 1; ++idx)
    {
        const auto currentValue{averagedValues.at(idx)};
        const auto nextValue{averagedValues.at(idx + 1)};

        if (isTrigger(dynamicData, currentValue, nextValue))
        {
            triggersIndexes.push_back(idx);
        }
    }

    return triggersIndexes;
}

bool DataAnalyzer::isTrigger(const DynamicData &dynamicData,
                             const uint16_t leftValue,
                             const uint16_t rightValue)
{
    const uint16_t threshold{scaleYToAdcWithinBounds(
        dynamicData, dynamicData.triggerThresholdSliderValue)};
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
