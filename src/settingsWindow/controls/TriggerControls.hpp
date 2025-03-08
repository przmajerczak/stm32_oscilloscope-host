#pragma once

#include <gtk/gtk.h>
#include "sharedData/DynamicData.hpp"

class TriggerControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getTriggerControlsContainer();

private:
    void prepareThresholdLabel(DynamicData &dynamicData);
    void prepareTriggerThresholdSlider(DynamicData &dynamicData);
    void prepareTriggerHorizontalPositionLabel(DynamicData &dynamicData);
    void prepareTriggerHorizontalPositionSlider(DynamicData &dynamicData);
    void prepareTriggerLabel(DynamicData &dynamicData);
    void prepareTriggerRisingEdgeButton(DynamicData &dynamicData);
    void prepareTriggerFallingEdgeButton(DynamicData &dynamicData);

    GtkWidget *thresholdLabel = nullptr;
    GtkWidget *triggerThresholdSlider = nullptr;
    GtkWidget *triggerHorizontalPositionLabel = nullptr;
    GtkWidget *triggerHorizontalPositionSlider = nullptr;
    GtkWidget *trigger_horizontal_position_spin_button = nullptr;
    GtkWidget *triggerLabel = nullptr;
    GtkWidget *triggerRisingEdgeButton = nullptr;
    GtkWidget *triggerFallingEdgeButton = nullptr;
    GtkWidget *trigger_spin_button = nullptr;
};
