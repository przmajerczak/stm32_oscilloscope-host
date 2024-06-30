#include "DataAnalyzer.hpp"
#include "DisplayHelper.hpp"
#include "SettingsWindow.hpp"
#include "constants.hpp"
#include "types.hpp"

void DataAnalyzer::handleData(const int current_value)
{
    rawRetrievedValues_leftHalf.pop_front();
    rawRetrievedValues_leftHalf.push_back(rawRetrievedValues_rightHalf.front());

    rawRetrievedValues_rightHalf.pop_front();
    rawRetrievedValues_rightHalf.push_back(current_value);

    if (triggerCondition())
    {
        SettingsWindow::notifyAboutThresholdTrigger();
        DisplayHelper::triggerDisplay(rawRetrievedValues_leftHalf,
                                      rawRetrievedValues_rightHalf);
    }
}

bool DataAnalyzer::triggerCondition()
{
    const int leftMiddleValue{rawRetrievedValues_leftHalf.back()};
    const int rightMiddleValue{rawRetrievedValues_rightHalf.front()};

    const uint16_t threshold{SettingsWindow::getTriggerThresholdSliderValue()};
    const ThresholdTrigger trigger{SettingsWindow::getThresholdTrigger()};

    if (trigger == ThresholdTrigger::RISING_EDGE)
    {
        return (leftMiddleValue < threshold and rightMiddleValue >= threshold);
    }

    if (trigger == ThresholdTrigger::FALLING_EDGE)
    {
        return (leftMiddleValue >= threshold and rightMiddleValue < threshold);
    }

    return false;
}
