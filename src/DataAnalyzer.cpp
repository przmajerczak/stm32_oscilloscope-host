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
    Timemarker tmarker{
        dynamicData.globalData.timemarkersData.totalDataAnalyzeDuration};

    dynamicData.channelData.adcValuesToDisplay =
        std::move(centerValuesOnTrigger(current_values, dynamicData));
}

AdcValues DataAnalyzer::centerValuesOnTrigger(
    const AdcValues &current_values,
    DynamicData &dynamicData) // TODO: change this name

{
    const auto averagedValues{averageAdcValues(dynamicData, current_values)};
    const auto triggersIndexes{detectTriggers(dynamicData, averagedValues)};

    AdcValues valuesToDisplay;
    if (averagedValues.size() == 0)
    {
        dynamicData.channelData.frequency_Hz = 0.0;
        return valuesToDisplay;
    }
    const double nanoseconds_per_sample{dynamicData.globalData.frame_duration_ns /
                                        averagedValues.size()};
    const uint32_t samples_to_display{
        dynamicData.globalData.horizontal_resolution_ns / nanoseconds_per_sample};

    dynamicData.channelData.nanoseconds_per_sample = nanoseconds_per_sample;
    dynamicData.channelData.frequency_Hz =
        calculateFrequency(triggersIndexes, nanoseconds_per_sample,
                           dynamicData.globalData.frame_duration_ns);

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
    const int shiftCountForTriggerCenter{static_cast<int>(
        selectedTrigger - (samples_to_display *
                           (dynamicData.channelData.trigger_horizontal_position /
                            X_DISPLAY_RESOLUTION)))};

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

AdcValues DataAnalyzer::averageAdcValues(const DynamicData &dynamicData,
                                         const AdcValues &current_values)
{
    const uint16_t averaging_window_size{
        dynamicData.channelData.averaging_window_size};

    if (averaging_window_size < 2)
    {
        return std::move(current_values);
    }

    if (averaging_window_size > current_values.size())
    {
        std::cerr << "Moving average window size exceeds number of samples per "
                     "frame - averaging not possible."
                  << std::endl;
        return std::move(current_values);
    }

    AdcValues averaged_values;
    averaged_values.resize(current_values.size() - averaging_window_size - 1);

    auto moving_average_window_front{current_values.begin()};
    auto moving_average_window_back{
        std::next(moving_average_window_front, averaging_window_size)};

    // TODO: make this algorithm more optimal by using real movinf window
    // with adding and subtracing front/back element at each iteration
    // instead of accumulating every time
    for (auto &averaged_value : averaged_values)
    {
        averaged_value = static_cast<AdcValues::value_type>(
            static_cast<double>(std::accumulate(moving_average_window_front,
                                                moving_average_window_back, 0)) /
            static_cast<double>(averaging_window_size));

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
        dynamicData, dynamicData.channelData.triggerThresholdSliderValue)};
    const ThresholdTrigger trigger{dynamicData.channelData.thresholdTrigger};

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

double DataAnalyzer::calculateFrequency(
    const std::vector<std::size_t> &triggersIndexes,
    const double nanoseconds_per_sample, const double frame_duration_ns)
{
    if (triggersIndexes.size() < 2)
    {
        return triggersIndexes.size() / frame_duration_ns;
    }

    std::vector<std::size_t> periods_between_triggers;
    periods_between_triggers.resize(triggersIndexes.size() - 1);

    for (std::size_t idx = 0; idx < periods_between_triggers.size(); ++idx)
    {
        periods_between_triggers.at(idx) =
            triggersIndexes.at(idx + 1) - triggersIndexes.at(idx);
    }

    std::sort(periods_between_triggers.begin(), periods_between_triggers.end());

    double median_signal_period;
    const std::size_t middle_period_index{periods_between_triggers.size() / 2};
    if (periods_between_triggers.size() & 0x01) // odd number of elements
    {
        median_signal_period = periods_between_triggers.at(middle_period_index);
    }
    else
    {
        const std::size_t lower_median_signal_period{
            periods_between_triggers.at(middle_period_index - 1)};
        const std::size_t upper_median_signal_period{
            periods_between_triggers.at(middle_period_index)};

        median_signal_period =
            (lower_median_signal_period + upper_median_signal_period) / 2;
    }

    double median_signal_period_ns =
        median_signal_period * nanoseconds_per_sample;
    constexpr double NANOSECONDS_PER_GHZ{1000000000.0};

    return NANOSECONDS_PER_GHZ / median_signal_period_ns;
}
