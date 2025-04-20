#include "VerticalBoundsData.hpp"

void VerticalBoundsData::notifyAboutLowerBoundChange(
    const uint16_t new_vertical_lower_bound)
{
    vertical_lower_bound = new_vertical_lower_bound;
    vertical_lower_bound_mV = scaleAdcTo_mV(new_vertical_lower_bound);

    if (vertical_lower_bound > vertical_upper_bound)
    {
        vertical_upper_bound = vertical_lower_bound;
    }
}

void VerticalBoundsData::notifyAboutUpperBoundChange(
    const uint16_t new_vertical_upper_bound)
{
    vertical_upper_bound = new_vertical_upper_bound;
    vertical_upper_bound_mV = scaleAdcTo_mV(new_vertical_upper_bound);

    if (vertical_upper_bound < vertical_lower_bound)
    {
        vertical_lower_bound = vertical_upper_bound;
    }
}

int16_t VerticalBoundsData::scaleAdcTo_mV(const uint16_t adc_value) const
{
    return static_cast<int16_t>(
               (ABSOULTE_VERTICAL_RESOLUTION_mV * static_cast<int16_t>(adc_value)) /
               static_cast<int16_t>(INPUT_SIGNAL_MAX)) +
           MIN_VOLTAGE_mV;
}
