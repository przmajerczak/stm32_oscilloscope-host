#pragma once

#include <gtk/gtk.h>

#include "CallbackData.hpp"
#include "sharedData/DynamicData.hpp"
#include "measurements/VerticalMeasurements.hpp"
#include "measurements/HorizontalMeasurements.hpp"

class MeasurementsControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getMeasurementsControlsContainer();

private:
    void prepareFrequencyLabel(DynamicData &dynamicData);
    void prepareMinVoltageLabel(DynamicData &dynamicData);
    void prepareMaxVoltageLabel(DynamicData &dynamicData);
    void prepareAvgVoltageLabel(DynamicData &dynamicData);

    GtkWidget *frequencyLabel;
    GtkWidget *minVoltageLabel;
    GtkWidget *maxVoltageLabel;
    GtkWidget *avgVoltageLabel;

    CallbackData<DynamicData> callbackDataForFrequencyLabel;
    CallbackData<DynamicData> callbackDataForMinVoltageLabel;
    CallbackData<DynamicData> callbackDataForMaxVoltageLabel;
    CallbackData<DynamicData> callbackDataForAvgVoltageLabel;

    VerticalMeasurements verticalMeasurements;
    HorizontalMeasurements horizontalMeasurements;
};
