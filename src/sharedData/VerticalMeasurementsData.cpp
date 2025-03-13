#include "VerticalMeasurementsData.hpp"
#include "utils.hpp"

void VerticalMeasurementsData::notifyAboutBaselineChange(
    const DynamicData &dynamicData, const float new_baseline)
{
    baseline = new_baseline;
    baseline_mV = scaleYToVoltage_mV(dynamicData, new_baseline);
}

void VerticalMeasurementsData::notifyAboutMeasurementChange(
    const DynamicData &dynamicData, const float new_measurement)
{
    measurement = new_measurement;
    measurement_mV = scaleYToVoltage_mV(dynamicData, new_measurement);
}
