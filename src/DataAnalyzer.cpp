#include "DataAnalyzer.hpp"
#include "debug/Timemarker.hpp"
#include "sharedData/constants.hpp"
#include "utils.hpp"
#include <numeric>

AdcValues DataAnalyzer::prepareData(const AdcValues &current_values,
                                    DynamicData &dynamicData,
                                    const ChannelId channelId)
{
    Timemarker tmarker{dynamicData.timemarkersData.totalDataAnalyzeDuration};

    const auto averaged_values{averageAdcValues(dynamicData, current_values)};
    const TriggersIndexes triggersIndexes{
        detectTriggers(dynamicData, averaged_values, channelId)};

    const AdcValues adc_values_to_display{
        centerValuesOnTrigger(averaged_values, dynamicData)};
    calculateMeasurements(dynamicData, adc_values_to_display, triggersIndexes,
                          channelId);

    return std::move(adc_values_to_display);
}

// TODO: change this name
AdcValues DataAnalyzer::centerValuesOnTrigger(const AdcValues &averaged_values,
                                              DynamicData &dynamicData)

{
    AdcValues valuesToDisplay;
    if (averaged_values.size() == 0)
    {
        return valuesToDisplay;
    }

    const uint32_t samples_to_display{
        static_cast<uint32_t>(dynamicData.horizontal_resolution_ns /
                              dynamicData.nanoseconds_per_sample)};

    valuesToDisplay.resize(samples_to_display);
    if (dynamicData.trigger_index == INVALID_VALUE)
    {
        for (std::size_t idx = 0; idx < samples_to_display; ++idx)
        {
            // TODO: copy this better with STL
            if (idx < averaged_values.size())
            {
                valuesToDisplay.at(idx) = averaged_values.at(idx);
            }
            else
            {
                valuesToDisplay.at(idx) = INVALID_VALUE;
            }
        }
        return valuesToDisplay;
    }

    const int shiftCountForTriggerCenter{static_cast<int>(
        dynamicData.trigger_index -
        (samples_to_display *
         (dynamicData.trigger_horizontal_position / X_DISPLAY_RESOLUTION)))};

    for (std::size_t idx = 0; idx < valuesToDisplay.size(); ++idx)
    {
        const std::size_t averaged_values_idx{idx + shiftCountForTriggerCenter};
        if (averaged_values_idx >= 0 and
            averaged_values_idx < averaged_values.size())
        {
            valuesToDisplay.at(idx) = averaged_values.at(averaged_values_idx);
        }
        else
        {
            valuesToDisplay.at(idx) = INVALID_VALUE;
        }
    }

    return valuesToDisplay;
}

AdcValues DataAnalyzer::averageAdcValues(DynamicData &dynamicData,
                                         const AdcValues &current_values)
{
    const uint16_t averaging_window_size{dynamicData.averaging_window_size};

    if (averaging_window_size < 2)
    {
        const double nanoseconds_per_sample{dynamicData.frame_duration_ns /
                                            current_values.size()};
        dynamicData.nanoseconds_per_sample = nanoseconds_per_sample;

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

    const double nanoseconds_per_sample{dynamicData.frame_duration_ns /
                                        averaged_values.size()};
    dynamicData.nanoseconds_per_sample = nanoseconds_per_sample;

    return std::move(averaged_values);
}

TriggersIndexes DataAnalyzer::detectTriggers(DynamicData &dynamicData,
                                             const AdcValues &averaged_values,
                                             const ChannelId channelId)
{
    TriggersIndexes triggersIndexes;

    for (std::size_t idx = 0; idx < averaged_values.size() - 1; ++idx)
    {
        const auto currentValue{averaged_values.at(idx)};
        const auto nextValue{averaged_values.at(idx + 1)};

        if (isTrigger(dynamicData, currentValue, nextValue))
        {
            triggersIndexes.push_back(idx);
        }
    }

    if (channelId == dynamicData.trigger_source)
    {
        dynamicData.trigger_index =
            triggersIndexes.empty() ? INVALID_VALUE
            : triggersIndexes.size() > 2
                ? triggersIndexes.at(triggersIndexes.size() / 2)
                : triggersIndexes.at(0);
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

double DataAnalyzer::calculateFrequency(const TriggersIndexes &triggersIndexes,
                                        const double nanoseconds_per_sample,
                                        const double frame_duration_ns)
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

void DataAnalyzer::calculateMeasurements(DynamicData &dynamicData,
                                         const AdcValues adc_values_to_display,
                                         const TriggersIndexes &triggersIndexes,
                                         const ChannelId channelId)
{
    dynamicData.frequency_Hz.at(channelId) =
        calculateFrequency(triggersIndexes, dynamicData.nanoseconds_per_sample,
                           dynamicData.frame_duration_ns);
}