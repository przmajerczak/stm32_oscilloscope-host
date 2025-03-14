#include "HorizontalMeasurementsData.hpp"
#include "utils.hpp"

void HorizontalMeasurementsData::notifyAboutBaselineChange(
    const DynamicData &dynamicData, const float new_baseline)
{
    baseline = new_baseline;
    baseline_ns = scaleXToNanoseconds(dynamicData, new_baseline);
}

void HorizontalMeasurementsData::notifyAboutMeasurementChange(
    const DynamicData &dynamicData, const float new_measurement)
{
    measurement = new_measurement;
    measurement_ns = scaleXToNanoseconds(dynamicData, new_measurement);
}
