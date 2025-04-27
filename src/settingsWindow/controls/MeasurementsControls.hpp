#pragma once

#include <gtk/gtk.h>

#include "CallbackData.hpp"
#include "measurements/HorizontalMeasurements.hpp"
#include "measurements/VerticalMeasurements.hpp"
#include "sharedData/DynamicData.hpp"
#include "sharedData/constants.hpp"

struct LabelsForSingleChannel
{
    GtkWidget *frequencyLabel;
    GtkWidget *minVoltageLabel;
    GtkWidget *maxVoltageLabel;
    GtkWidget *avgVoltageLabel;
};

using LabelsPointers = std::array<LabelsForSingleChannel, NUMBER_OF_CHANNELS>;

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

    GtkWidget *getMeasurementsGridContainer();

    LabelsPointers labels_pointers;

    CallbackData<DynamicData> callbackDataForFrequencyLabel;
    CallbackData<DynamicData> callbackDataForMinVoltageLabel;
    CallbackData<DynamicData> callbackDataForMaxVoltageLabel;
    CallbackData<DynamicData> callbackDataForAvgVoltageLabel;

    VerticalMeasurements verticalMeasurements;
    HorizontalMeasurements horizontalMeasurements;
};
