#include "DataAnalyzer.hpp"
#include "DisplayHelper.hpp"
#include "SettingsWindow.hpp"
#include "constants.hpp"
#include "types.hpp"

constexpr uint16_t defaultThreshold{(INPUT_RESOLUTION / 2)};

void DataAnalyzer::handleData(const int current_value)
{
    raw_retrieved_values.pop_back();
    raw_retrieved_values.push_front(current_value);

    if (triggerCondition(defaultThreshold, ThresholdTrigger::FALLING_EDGE))
    {
        SettingsWindow::notifyAboutThresholdTrigger();
        DisplayHelper::triggerDisplay(raw_retrieved_values);

    }
}

bool DataAnalyzer::triggerCondition(const uint16_t threshold, const ThresholdTrigger trigger)
{
    const int current_value{*(raw_retrieved_values.begin())};
    const int previous_value{*(std::next(raw_retrieved_values.begin(), 1))};

    if (trigger == ThresholdTrigger::RISING_EDGE)
    {
        return (current_value < threshold and previous_value >= threshold);
    }

    if (trigger == ThresholdTrigger::FALLING_EDGE)
    {
        return (current_value >= threshold and previous_value < threshold);
    }

    return false;
}
