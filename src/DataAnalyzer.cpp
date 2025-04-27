#include "DataAnalyzer.hpp"
#include "debug/Timemarker.hpp"
#include "sharedData/constants.hpp"
#include "utils.hpp"
#include <algorithm>
#include <numeric>

AdcValues DataAnalyzer::prepareData(const AdcValues &current_values,
                                    DynamicData &dynamicData,
                                    const ChannelId channelId)
{
    Timemarker tmarker{dynamicData.timemarkersData.totalDataAnalyzeDuration};

    const auto averaged_values{averageAdcValues(dynamicData, current_values)};
    const TriggersIndexes triggersIndexes{
        detectTriggers(dynamicData, averaged_values, channelId)};

    calculateMeasurements(dynamicData, averaged_values, triggersIndexes,
                          channelId);

    return std::move(averaged_values);
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
    SignalMeasurementsData &signalMeasurementsData{
        dynamicData.signalMeasurementsData.at(channelId)};

    signalMeasurementsData.frequency_Hz =
        calculateFrequency(triggersIndexes, dynamicData.nanoseconds_per_sample,
                           dynamicData.frame_duration_ns);

    const auto min_value{std::min_element(adc_values_to_display.begin(),
                                          adc_values_to_display.end())};

    signalMeasurementsData.min_value =
        (min_value != adc_values_to_display.end()) ? *min_value : INVALID_VALUE;

    const auto max_value{std::max_element(adc_values_to_display.begin(),
                                          adc_values_to_display.end())};

    signalMeasurementsData.max_value =
        (max_value != adc_values_to_display.end()) ? *max_value : INVALID_VALUE;

    signalMeasurementsData.amplitude = max_value - min_value;

    if (adc_values_to_display.empty())
    {
        signalMeasurementsData.average_value = INVALID_VALUE;
    }
    else
    {
        const auto average_value{std::accumulate(adc_values_to_display.begin(),
                                                 adc_values_to_display.end(), 0) /
                                 adc_values_to_display.size()};
        signalMeasurementsData.average_value = average_value;
    }
}