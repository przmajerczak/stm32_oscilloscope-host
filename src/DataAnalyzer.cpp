#include "DataAnalyzer.hpp"
#include "DisplayHelper.hpp"
#include "SettingsWindow.hpp"
#include "constants.hpp"
#include "types.hpp"

void DataAnalyzer::handleData(const int current_value)
{
    raw_retrieved_values.pop_back();
    raw_retrieved_values.push_front(current_value);

    if (triggerCondition())
    {
        SettingsWindow::notifyAboutThresholdTrigger();
        DisplayHelper::triggerDisplay(raw_retrieved_values);
    }
}

bool DataAnalyzer::triggerCondition()
{
    const int current_value{*(raw_retrieved_values.begin())};
    const int previous_value{*(std::next(raw_retrieved_values.begin(), 1))};

    const uint16_t threshold{SettingsWindow::getTriggerThresholdSliderValue()};
    const ThresholdTrigger trigger{SettingsWindow::getThresholdTrigger()};

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
