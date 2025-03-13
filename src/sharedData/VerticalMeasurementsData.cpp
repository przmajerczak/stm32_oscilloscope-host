#include "VerticalMeasurementsData.hpp"

void VerticalMeasurementsData::notifyAboutBaselineChange(const float new_baseline)
{
    notifyAboutAChange(new_baseline);
}

void VerticalMeasurementsData::notifyAboutMeasurementChange(const float new_measurement)
{
    notifyAboutBChange(new_measurement);
}
