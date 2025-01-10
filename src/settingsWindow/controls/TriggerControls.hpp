#pragma once

#include <gtk/gtk.h>
#include "types.hpp"

class TriggerControls
{
public:
    static void prepare();
    static void triggerThresholdSliderOnChangeAction(GtkRange *range);
    static void onTriggerRisingEdgeButtonClicked(GtkWidget *button,
                                                 gpointer data);
    static void onTriggerFallingEdgeButtonClicked(GtkWidget *button,
                                                  gpointer data);

    static GtkWidget *getThresholdLabel();
    static GtkWidget *getTriggerThresholdSlider();
    static GtkWidget *getTriggerLabel();
    static GtkWidget *getTriggerRisingEdgeButton();
    static GtkWidget *getTriggerFallingEdgeButton();

    static uint16_t getTriggerThresholdY();
    static ThresholdTrigger getThresholdTrigger();
};
