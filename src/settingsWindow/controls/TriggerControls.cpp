#include "TriggerControls.hpp"

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

void TriggerControls::prepare()
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

GtkWidget *TriggerControls::getTriggerControlsContainer()
{
    GtkWidget *triggerControlsGrid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), thresholdLabel, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerThresholdSlider, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerLabel, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerRisingEdgeButton, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerFallingEdgeButton, 1, 3, 1, 1);

    return triggerControlsGrid;
}

void TriggerControls::triggerThresholdSliderOnChangeAction(GtkRange *range)
{
    triggerThresholdSliderValue =
        static_cast<uint16_t>(gtk_range_get_value(range));
}

void TriggerControls::onTriggerRisingEdgeButtonClicked(GtkWidget *button,
                                                       gpointer data)
{
    thresholdTrigger = ThresholdTrigger::RISING_EDGE;
}
void TriggerControls::onTriggerFallingEdgeButtonClicked(GtkWidget *button,
                                                        gpointer data)
{
    thresholdTrigger = ThresholdTrigger::FALLING_EDGE;
}

uint16_t TriggerControls::getTriggerThresholdY() { return triggerThresholdSliderValue; }
ThresholdTrigger TriggerControls::getThresholdTrigger() { return thresholdTrigger; }