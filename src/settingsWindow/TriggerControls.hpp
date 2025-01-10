#pragma once

#include <gtk/gtk.h>
#include "constants.hpp"

namespace
{
    GtkWidget *thresholdLabel = nullptr;
    GtkWidget *triggerThresholdSlider = nullptr;
    GtkWidget *triggerLabel = nullptr;
    GtkWidget *triggerRisingEdgeButton = nullptr;
    GtkWidget *triggerFallingEdgeButton = nullptr;

    uint16_t triggerThresholdSliderValue = DEFAULT_TRIGGER_THRESHOLD;
    ThresholdTrigger thresholdTrigger{ThresholdTrigger::FALLING_EDGE};
} // namespace

class TriggerControls
{
public:
    static void prepare()
    {
        thresholdLabel = gtk_label_new("Threshold trigger value:");
        gtk_widget_set_hexpand(thresholdLabel, TRUE);

        triggerThresholdSlider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,
                                                          0, Y_DISPLAY_RESOLUTION, 1);
        gtk_widget_set_hexpand(triggerThresholdSlider, TRUE);
        gtk_scale_set_draw_value(GTK_SCALE(triggerThresholdSlider), TRUE);
        gtk_range_set_value(GTK_RANGE(triggerThresholdSlider),
                            DEFAULT_TRIGGER_THRESHOLD);

        g_signal_connect(triggerThresholdSlider, "value-changed",
                         G_CALLBACK(triggerThresholdSliderOnChangeAction), nullptr);

        triggerLabel = gtk_label_new("Threshold trigger edge:");
        gtk_widget_set_hexpand(triggerLabel, TRUE);

        triggerRisingEdgeButton = gtk_button_new_with_label("__/‾‾");
        g_signal_connect(triggerRisingEdgeButton, "clicked",
                         G_CALLBACK(onTriggerRisingEdgeButtonClicked), NULL);

        triggerFallingEdgeButton = gtk_button_new_with_label("‾‾\\__");
        g_signal_connect(triggerFallingEdgeButton, "clicked",
                         G_CALLBACK(onTriggerFallingEdgeButtonClicked), NULL);
    }

    static void triggerThresholdSliderOnChangeAction(GtkRange *range)
    {
        triggerThresholdSliderValue =
            static_cast<uint16_t>(gtk_range_get_value(range));
    }

    static void onTriggerRisingEdgeButtonClicked(GtkWidget *button,
                                                 gpointer data)
    {
        thresholdTrigger = ThresholdTrigger::RISING_EDGE;
    }
    static void onTriggerFallingEdgeButtonClicked(GtkWidget *button,
                                                  gpointer data)
    {
        thresholdTrigger = ThresholdTrigger::FALLING_EDGE;
    }

    static GtkWidget *getThresholdLabel() { return thresholdLabel; }
    static GtkWidget *getTriggerThresholdSlider() { return triggerThresholdSlider; }
    static GtkWidget *getTriggerLabel() { return triggerLabel; }
    static GtkWidget *getTriggerRisingEdgeButton() { return triggerRisingEdgeButton; }
    static GtkWidget *getTriggerFallingEdgeButton() { return triggerFallingEdgeButton; }

    static uint16_t getTriggerThresholdY() { return triggerThresholdSliderValue; }
    static ThresholdTrigger getThresholdTrigger() { return thresholdTrigger; }
};
