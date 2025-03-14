#pragma once

#include "constants.hpp"

class DynamicData;

class HorizontalMeasurementsData
{
public:
    bool active{false};

    float &baselineIndicator() { return baseline; }
    float &measurementIndicator() { return measurement; }
    // float &baselineIndicator_ns() { return baseline_ns; }
    // float &measurementIndicator_ns() { return measurement_ns; }

    const float baselineIndicator() const { return baseline; }
    const float measurementIndicator() const { return measurement; }
    // const float baselineIndicator_ns() const { return baseline_ns; }
    // const float measurementIndicator_ns() const { return measurement_ns; }

    void notifyAboutBaselineChange(const DynamicData &dynamicData,
                                   const float new_baseline);
    void notifyAboutMeasurementChange(const DynamicData &dynamicData,
                                      const float new_measurement);

private:
    float baseline{X_DISPLAY_RESOLUTION * 0.333};
    float measurement{X_DISPLAY_RESOLUTION * 0.666};
    // float baseline_ns{};
    // float measurement_ns{};
};
