#pragma once

#include <gtk/gtk.h>

#include "controls/AveragingControls.hpp"
#include "controls/DebugDataControls.hpp"
#include "controls/HorizontalResolutionControls.hpp"
#include "controls/MeasurementsControls.hpp"
#include "controls/TriggerControls.hpp"
#include "controls/VerticalBoundControls.hpp"
#include "sharedData/DynamicData.hpp"

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
    MeasurementsControls measurementsControls;
    DebugDataControls debugDataControls;
};
