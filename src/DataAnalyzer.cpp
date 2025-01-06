#include "DataAnalyzer.hpp"
#include "DisplayHelper.hpp"
#include "SettingsWindow.hpp"
#include "constants.hpp"
#include "types.hpp"

void DataAnalyzer::handleData(const RawValuesContainer &current_values)
{
    RawValuesContainer valuesToDisplay{centerValuesOnTrigger(current_values)};

    SettingsWindow::notifyAboutThresholdTrigger();
    DisplayHelper::triggerDisplay(valuesToDisplay);
}

RawValuesContainer DataAnalyzer::centerValuesOnTrigger(const RawValuesContainer &current_values)
{
    const auto triggersIndexes{detectTriggers(current_values)};

    if (triggersIndexes.size() == 0)
    {
        return current_values;
    }

    std::size_t selectedTrigger{triggersIndexes.at(triggersIndexes.size() / 2)};
    const int shiftCountForTriggerCenter{static_cast<int>(selectedTrigger - (X_DISPLAY_RESOLUTION / 2))};

    RawValuesContainer valuesToDisplay;
    valuesToDisplay.resize(current_values.size());

    for (std::size_t idx = 0; idx < valuesToDisplay.size(); ++idx)
    {
        const std::size_t current_values_idx{idx + shiftCountForTriggerCenter + 2};
        if (current_values_idx >= 0 and current_values_idx < current_values.size())
        {
            valuesToDisplay.at(idx) = current_values.at(current_values_idx);
        }
        else
        {
            valuesToDisplay.at(idx) = INVALID_VALUE;
        }
    }

    return valuesToDisplay;
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
