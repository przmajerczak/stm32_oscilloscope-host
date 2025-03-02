#include "TriggerControls.hpp"

#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"

void triggerThresholdSliderOnChangeAction(GtkRange *range, gpointer data)
{
    uint16_t *triggerThresholdSliderValue_ptr = (uint16_t *)data;
    *triggerThresholdSliderValue_ptr =
        static_cast<uint16_t>(gtk_range_get_value(range));
}

void onTriggerRisingEdgeButtonClicked(GtkWidget *button, gpointer data)
{
    ThresholdTrigger *thresholdTrigger_ptr = (ThresholdTrigger *)data;

    *thresholdTrigger_ptr = ThresholdTrigger::RISING_EDGE;
}
void onTriggerFallingEdgeButtonClicked(GtkWidget *button, gpointer data)
{
    ThresholdTrigger *thresholdTrigger_ptr = (ThresholdTrigger *)data;

    *thresholdTrigger_ptr = ThresholdTrigger::FALLING_EDGE;
}

void TriggerControls::prepare(DynamicData &dynamicData)
{
    prepareThresholdLabel(dynamicData);
    prepareTriggerThresholdSlider(dynamicData);
    prepareTriggerLabel(dynamicData);
    prepareTriggerRisingEdgeButton(dynamicData);
    prepareTriggerFallingEdgeButton(dynamicData);
}

void TriggerControls::prepareThresholdLabel(DynamicData &dynamicData)
{
    thresholdLabel = gtk_label_new("Threshold trigger value:");
    gtk_widget_set_hexpand(thresholdLabel, TRUE);
}

void TriggerControls::prepareTriggerThresholdSlider(DynamicData &dynamicData)
{
    triggerThresholdSlider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,
                                                      0, Y_DISPLAY_RESOLUTION, 1);
    gtk_widget_set_hexpand(triggerThresholdSlider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(triggerThresholdSlider), FALSE);
    gtk_range_set_value(GTK_RANGE(triggerThresholdSlider),
                        DEFAULT_TRIGGER_THRESHOLD);

    g_signal_connect(triggerThresholdSlider, "value-changed",
                     G_CALLBACK(triggerThresholdSliderOnChangeAction),
                     &(dynamicData.triggerThresholdSliderValue));
}

void TriggerControls::prepareTriggerLabel(DynamicData &dynamicData)
{
    triggerLabel = gtk_label_new("Threshold trigger edge:");
    gtk_widget_set_hexpand(triggerLabel, TRUE);
}

void TriggerControls::prepareTriggerRisingEdgeButton(DynamicData &dynamicData)
{
    triggerRisingEdgeButton = gtk_button_new_with_label("__/‾‾");
    g_signal_connect(triggerRisingEdgeButton, "clicked",
                     G_CALLBACK(onTriggerRisingEdgeButtonClicked),
                     &(dynamicData.thresholdTrigger));
}

void TriggerControls::prepareTriggerFallingEdgeButton(DynamicData &dynamicData)
{
    triggerFallingEdgeButton = gtk_button_new_with_label("‾‾\\__");
    g_signal_connect(triggerFallingEdgeButton, "clicked",
                     G_CALLBACK(onTriggerFallingEdgeButtonClicked),
                     &(dynamicData.thresholdTrigger));
}

GtkWidget *TriggerControls::getTriggerControlsContainer()
{
    GtkWidget *triggerControlsExpander = gtk_expander_new("Trigger controls");
    gtk_expander_set_expanded(GTK_EXPANDER(triggerControlsExpander), TRUE);

    GtkWidget *triggerControlsGrid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), thresholdLabel, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerThresholdSlider, 0, 1,
                    2, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerLabel, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerRisingEdgeButton, 0, 3,
                    1, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerFallingEdgeButton, 1, 3,
                    1, 1);

    gtk_container_add(GTK_CONTAINER(triggerControlsExpander), triggerControlsGrid);
    return triggerControlsExpander;
}
