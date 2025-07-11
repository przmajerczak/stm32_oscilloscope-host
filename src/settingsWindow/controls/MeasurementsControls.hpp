#pragma once

#include <gtk/gtk.h>

#include "measurements/HorizontalMeasurements.hpp"
#include "measurements/VerticalMeasurements.hpp"
#include "sharedData/DynamicData.hpp"
#include "sharedData/constants.hpp"

struct LabelsForSingleChannel
{
    GtkWidget *titleLabel;
    GtkWidget *frequencyLabel;
    GtkWidget *maxVoltageLabel;
    GtkWidget *avgVoltageLabel;
    GtkWidget *minVoltageLabel;
    GtkWidget *amplitudeLabel;
};

using LabelsPointers = std::array<LabelsForSingleChannel, NUMBER_OF_CHANNELS>;

struct LabelsAndDynamicData
{
    DynamicData *dynamicData;
    LabelsPointers *labels_pointers;
};

class MeasurementsControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getMeasurementsControlsContainer();

private:
    void prepareLabels(DynamicData &dynamicData);

    GtkWidget *getMeasurementsGridContainer();

    LabelsPointers labels_pointers;
    LabelsAndDynamicData labels_and_dynamic_data;

    VerticalMeasurements verticalMeasurements;
    HorizontalMeasurements horizontalMeasurements;
};
