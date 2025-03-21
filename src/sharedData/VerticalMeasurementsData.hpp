#pragma once

#include "constants.hpp"

class DynamicData;

class VerticalMeasurementsData
{
public:
    bool active{false};

    float &baselineIndicator() { return baseline; }
    float &measurementIndicator() { return measurement; }
    float &baselineIndicator_mV() { return baseline_mV; }
    float &measurementIndicator_mV() { return measurement_mV; }

    const float baselineIndicator() const { return baseline; }
    const float measurementIndicator() const { return measurement; }
    const float baselineIndicator_mV() const { return baseline_mV; }
    const float measurementIndicator_mV() const { return measurement_mV; }

    void notifyAboutBaselineChange(const DynamicData &dynamicData,
                                   const float new_baseline);
    void notifyAboutMeasurementChange(const DynamicData &dynamicData,
                                      const float new_measurement);
    void recalculateValues(const DynamicData &dynamicData);

private:
    float baseline{Y_DISPLAY_RESOLUTION * 0.333};
    float measurement{Y_DISPLAY_RESOLUTION * 0.666};
    float baseline_mV{MIN_VOLTAGE_mV};
    float measurement_mV{MAX_VOLTAGE_mV};
};
