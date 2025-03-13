#include "VerticalMeasurementsData.hpp"

void VerticalMeasurementsData::notifyAboutBaselineChange(const float new_baseline)
{
    baseline = new_baseline;
    baseline_mV = scaleYToVoltage_mV(new_baseline);
}

void VerticalMeasurementsData::notifyAboutMeasurementChange(const float new_measurement)
{
    measurement = new_measurement;
    measurement_mV = scaleYToVoltage_mV(new_measurement);
}
