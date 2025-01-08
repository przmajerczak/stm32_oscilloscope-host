#pragma once

#include <gtk/gtk.h>
#include "types.hpp"

class SettingsWindow
{

public:
    static void run();
    static void notifyAboutThresholdTrigger();
    static uint16_t getTriggerThresholdSliderValue();
    static ThresholdTrigger getThresholdTrigger();
    static float getVerticalLowerBoundValue();
    static float getVerticalUpperBoundValue();

private:
    static void init();
    static gboolean frequencyLabelTimeoutAction(gpointer widgetPtr);
    static void triggerThresholdSliderOnChangeAction(GtkRange *range);
    static void onTriggerRisingEdgeButtonClicked(GtkWidget *button, gpointer data);
    static void onTriggerFallingEdgeButtonClicked(GtkWidget *button, gpointer data);
    static void verticalLowerBoundSliderOnChangeAction(GtkRange *range);
    static void verticalUpperBoundSliderOnChangeAction(GtkRange *range);
};
