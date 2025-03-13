#pragma once

#include "VerticalDataBase.hpp"

class VerticalMeasurementsData : public VerticalDataBase
{
public:
    bool active{false};

    float &baselineIndicator() { return a; }
    float &measurementIndicator() { return b; }
    float &baselineIndicator_mV() { return a_mV; }
    float &measurementIndicator_mV() { return b_mV; }

    const float baselineIndicator() const { return a; }
    const float measurementIndicator() const { return b; }
    const float baselineIndicator_mV() const { return a_mV; }
    const float measurementIndicator_mV() const { return b_mV; }

    void notifyAboutBaselineChange(const float new_baseline);
    void notifyAboutMeasurementChange(const float new_measurement);
};
