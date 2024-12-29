#include "DataAnalyzer.hpp"
#include "DisplayHelper.hpp"
#include "SettingsWindow.hpp"
#include "constants.hpp"
#include "types.hpp"

void DataAnalyzer::handleData(const RawValuesContainer &current_values)
{
    [[maybe_unused]] const auto triggersIndexes{detectTriggers(current_values)};

    SettingsWindow::notifyAboutThresholdTrigger();
    DisplayHelper::triggerDisplay(current_values);
}

std::vector<std::size_t> DataAnalyzer::detectTriggers(const RawValuesContainer &current_values)
{
    std::vector<std::size_t> triggersIndexes;

    for (std::size_t idx = 0; idx < current_values.size() - 1; ++idx)
    {
        const auto currentValue{current_values.at(idx)};
        const auto nextValue{current_values.at(idx + 1)};

        if (isTrigger(currentValue, nextValue))
        {
            triggersIndexes.push_back(idx);
        }
    }

    return triggersIndexes;
}

bool DataAnalyzer::isTrigger(const uint16_t leftValue, const uint16_t rightValue)
{
    const uint16_t threshold{SettingsWindow::getTriggerThresholdSliderValue()};
    const ThresholdTrigger trigger{SettingsWindow::getThresholdTrigger()};

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
