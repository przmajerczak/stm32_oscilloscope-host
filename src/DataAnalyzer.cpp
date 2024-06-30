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
    // TODO: split list into two for faster std::next calls
    const int middleValue{*(std::next(raw_retrieved_values.begin(),
                                      raw_retrieved_values.size() / 2))};
    const int nextToMiddleValue{*(std::next(
        raw_retrieved_values.begin(), 1 + (raw_retrieved_values.size() / 2)))};

    const uint16_t threshold{SettingsWindow::getTriggerThresholdSliderValue()};
    const ThresholdTrigger trigger{SettingsWindow::getThresholdTrigger()};

    if (trigger == ThresholdTrigger::RISING_EDGE)
    {
        return (middleValue < threshold and nextToMiddleValue >= threshold);
    }

    if (trigger == ThresholdTrigger::FALLING_EDGE)
    {
        return (middleValue >= threshold and nextToMiddleValue < threshold);
    }

    return false;
}
