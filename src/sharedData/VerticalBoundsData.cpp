#include "VerticalBoundsData.hpp"

void VerticalBoundsData::notifyAboutLowerBoundChange(const float new_vertical_lower_bound)
{
    vertical_lower_bound = new_vertical_lower_bound;
    vertical_lower_bound_mV = scaleAdcTo_mV(new_vertical_lower_bound);

    if (vertical_lower_bound > vertical_upper_bound)
    {
        vertical_upper_bound = vertical_lower_bound;
    }
}

void VerticalBoundsData::notifyAboutUpperBoundChange(const float new_vertical_upper_bound)
{
    vertical_upper_bound = new_vertical_upper_bound;
    vertical_upper_bound_mV = scaleAdcTo_mV(new_vertical_upper_bound);

    if (vertical_upper_bound < vertical_lower_bound)
    {
        vertical_lower_bound = vertical_upper_bound;
    }
}

float VerticalBoundsData::scaleAdcTo_mV(const uint16_t adc_value) const
{
    return static_cast<float>(
        (ABSOULTE_VERTICAL_RESOLUTION_mV * static_cast<float>(adc_value) +
         MIN_VOLTAGE_mV) /
        static_cast<float>(INPUT_SIGNAL_MAX));
}