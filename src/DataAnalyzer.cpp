#include "DataAnalyzer.hpp"
#include "DisplayHelper.hpp"
#include "constants.hpp"
#include "types.hpp"

void DataAnalyzer::handleData(const int current_value)
{
    raw_retrieved_values.pop_back();
    raw_retrieved_values.push_front(current_value);

    if (triggerCondition(ThresholdTrigger::FALLING_EDGE))
    {
        DisplayHelper::triggerDisplay(raw_retrieved_values);
    }
}

bool DataAnalyzer::triggerCondition(const ThresholdTrigger trigger)
{
    const int current_value{*(raw_retrieved_values.begin())};
    const int previous_value{*(std::next(raw_retrieved_values.begin(), 1))};

    constexpr int thres{INPUT_RESOLUTION / 2};

    if (trigger == ThresholdTrigger::RISING_EDGE)
    {
        return (current_value < thres and previous_value >= thres);
    }

    if (trigger == ThresholdTrigger::FALLING_EDGE)
    {
        return (current_value >= thres and previous_value < thres);
    }

    return false;
}
