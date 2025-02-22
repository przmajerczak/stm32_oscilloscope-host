#pragma once

#include <gtk/gtk.h>
#include "sharedData/DynamicData.hpp"

class TriggerControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getTriggerControlsContainer();

private:
    GtkWidget *thresholdLabel = nullptr;
    GtkWidget *triggerThresholdSlider = nullptr;
    GtkWidget *triggerLabel = nullptr;
    GtkWidget *triggerRisingEdgeButton = nullptr;
    GtkWidget *triggerFallingEdgeButton = nullptr;
};
