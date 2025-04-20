#include "VerticalBoundsData.hpp"

void VerticalBoundsData::notifyAboutLowerBoundChange(
    const uint16_t new_vertical_lower_bound_mV)
{
    vertical_lower_bound_mV = new_vertical_lower_bound_mV;
    vertical_lower_bound = scaleMillivoltsToAdc(new_vertical_lower_bound_mV);

    if (vertical_lower_bound_mV > vertical_upper_bound_mV)
    {
        vertical_upper_bound_mV = vertical_lower_bound_mV;
    }
}

void VerticalBoundsData::notifyAboutUpperBoundChange(
    const uint16_t new_vertical_upper_bound_mV)
{
    vertical_upper_bound_mV = new_vertical_upper_bound_mV;
    vertical_upper_bound = scaleMillivoltsToAdc(new_vertical_upper_bound_mV);

    if (vertical_upper_bound_mV < vertical_lower_bound_mV)
    {
        vertical_lower_bound_mV = vertical_upper_bound_mV;
    }
}

int16_t VerticalBoundsData::scaleAdcTo_mV(const uint16_t adc_value) const
{
    return static_cast<int16_t>((ABSOULTE_VERTICAL_RESOLUTION_mV *
                                 static_cast<int16_t>(adc_value)) /
                                static_cast<int16_t>(INPUT_SIGNAL_MAX)) +
           MIN_VOLTAGE_mV;
}

uint16_t VerticalBoundsData::scaleMillivoltsToAdc(const int16_t value_mV) const
{
    return (value_mV - MIN_VOLTAGE_mV) *
           (static_cast<float>(INPUT_SIGNAL_MAX) /
            static_cast<float>(ABSOULTE_VERTICAL_RESOLUTION_mV));
}
