#include "DataAnalyzer.hpp"
#include "DisplayHelper.hpp"
#include "constants.hpp"

void DataAnalyzer::handleData(const int current_value)
{
    raw_retrieved_values.pop_back();
    raw_retrieved_values.push_front(current_value);

    if (triggerCondition())
    {
        DisplayHelper::triggerDisplay(raw_retrieved_values);
    }
}

bool DataAnalyzer::triggerCondition()
{
    const int current_value{*(raw_retrieved_values.begin())};
    const int previous_value{*(std::next(raw_retrieved_values.begin(), 1))};

    constexpr int thres{INPUT_RESOLUTION / 2};

    return (current_value >= thres and previous_value < thres);
}
