#pragma once

#include "sharedData/ChannelData.hpp"
#include <gtk/gtk.h>

class TriggerControls
{
public:
    void prepare(ChannelData &channelData);
    GtkWidget *getTriggerControlsContainer();

private:
    void prepareThresholdLabel();
    void prepareTriggerThresholdSlider(ChannelData &channelData);
    void prepareTriggerHorizontalPositionLabel();
    void prepareTriggerHorizontalPositionSlider(ChannelData &channelData);
    void prepareTriggerLabel();
    void prepareTriggerRisingEdgeButton(ChannelData &channelData);
    void prepareTriggerFallingEdgeButton(ChannelData &channelData);

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
