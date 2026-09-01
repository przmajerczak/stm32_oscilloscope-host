#include "VerticalBoundsData.hpp"

void VerticalBoundsData::notifyAboutLowerBoundChange(
    const float new_vertical_lower_bound_mV)
{
    vertical_lower_bound_mV = new_vertical_lower_bound_mV;
    vertical_lower_bound = scaleMillivoltsToAdc(new_vertical_lower_bound_mV);

    if (vertical_lower_bound_mV > vertical_upper_bound_mV)
    {
        vertical_upper_bound_mV = vertical_lower_bound_mV;
    }
}

void VerticalBoundsData::notifyAboutUpperBoundChange(
    const float new_vertical_upper_bound_mV)
{
    vertical_upper_bound_mV = new_vertical_upper_bound_mV;
    vertical_upper_bound = scaleMillivoltsToAdc(new_vertical_upper_bound_mV);

    if (vertical_upper_bound_mV < vertical_lower_bound_mV)
    {
        vertical_lower_bound_mV = vertical_upper_bound_mV;
    }
}

float VerticalBoundsData::scaleAdcTo_mV(const uint16_t adc_value) const
{
    return (ABSOULTE_VERTICAL_RESOLUTION_mV *
            static_cast<float>(adc_value)) /
               static_cast<float>(INPUT_SIGNAL_MAX) +
           MIN_VOLTAGE_mV;
}

uint16_t VerticalBoundsData::scaleMillivoltsToAdc(const float value_mV) const
{
    return (value_mV - MIN_VOLTAGE_mV) *
           (static_cast<float>(INPUT_SIGNAL_MAX) /
            ABSOULTE_VERTICAL_RESOLUTION_mV);
}
