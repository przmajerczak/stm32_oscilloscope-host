#pragma once

#include <gtk/gtk.h>

#include "controls/measurements/HorizontalMeasurements.hpp"
#include "controls/measurements/VerticalMeasurements.hpp"
#include "controls/AveragingControls.hpp"
#include "controls/DebugDataControls.hpp"
#include "controls/HorizontalResolutionControls.hpp"
#include "controls/MeasurementsControls.hpp"
#include "controls/TriggerControls.hpp"
#include "controls/VerticalBoundControls.hpp"

class DynamicData;

class SettingsWindow
{
public:
    void runAsSeparateThread(DynamicData &dynamicData);

private:
    void configure();
    void fill(DynamicData &dynamicData);

    GtkWidget *window;

    HorizontalResolutionControls horizontalResolutionControls;
    TriggerControls triggerControls;
    AveragingControls averagingControls;
    VerticalBoundControls verticalBoundControls;
    VerticalMeasurements verticalMeasurements;
    HorizontalMeasurements horizontalMeasurements;
    MeasurementsControls measurementsControls;
    DebugDataControls debugDataControls;
};
