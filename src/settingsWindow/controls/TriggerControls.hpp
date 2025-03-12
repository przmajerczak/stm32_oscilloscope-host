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
    void prepareTriggerSourceSelection(DynamicData &dynamicData);

    GtkWidget *trigger_threshold_label = nullptr;
    GtkWidget *trigger_threshold_slider = nullptr;
    GtkWidget *trigger_horizontal_position_label = nullptr;
    GtkWidget *trigger_horizontal_position_slider = nullptr;
    GtkWidget *trigger_horizontal_position_spin_button = nullptr;
    GtkWidget *trigger_edge_label = nullptr;
    GtkWidget *trigger_rising_edge_button = nullptr;
    GtkWidget *trigger_falling_edge_button = nullptr;
    GtkWidget *trigger_spin_button = nullptr;
    GtkWidget *trigger_source_label = nullptr;
    GtkWidget *trigger_source_combo = nullptr;
};
