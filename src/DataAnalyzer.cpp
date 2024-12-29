#include "DataAnalyzer.hpp"
#include "DisplayHelper.hpp"
#include "SettingsWindow.hpp"
#include "constants.hpp"
#include "types.hpp"

void DataAnalyzer::handleData(const std::vector<uint16_t> current_values)
{
    for (int i = 0; i < current_values.size(); ++i)
    {
        rawRetrievedValues.pop_front();
        rawRetrievedValues.push_back(current_values.at(i));
    }

    // if (triggerCondition())
    {
        SettingsWindow::notifyAboutThresholdTrigger();
        DisplayHelper::triggerDisplay(rawRetrievedValues);
    }
}

bool DataAnalyzer::triggerCondition()
{
    /*
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
*/
    return false;
}
