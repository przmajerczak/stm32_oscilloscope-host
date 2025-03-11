#include "TriggerControls.hpp"

#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"

void triggerThresholdSliderOnChangeAction(GtkRange *range, gpointer data)
{
    uint16_t *triggerThresholdSliderValue_ptr = (uint16_t *)data;
    *triggerThresholdSliderValue_ptr =
        static_cast<uint16_t>(gtk_range_get_value(range));
}

void triggerHorizontalPositionSliderOnChangeAction(GtkRange *range,
                                                   gpointer data)
{
    DynamicData *dynamicData = (DynamicData *)data;
    dynamicData->trigger_horizontal_position =
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

static void onTriggerSourceChanged(GtkComboBox *widget, gpointer user_data)
{
    std::size_t *trigger_source = static_cast<std::size_t *>(user_data);

    *trigger_source = gtk_combo_box_get_active(widget);
}

void TriggerControls::prepare(DynamicData &dynamicData)
{
    prepareThresholdLabel(dynamicData);
    prepareTriggerThresholdSlider(dynamicData);
    prepareTriggerHorizontalPositionLabel(dynamicData);
    prepareTriggerHorizontalPositionSlider(dynamicData);
    prepareTriggerLabel(dynamicData);
    prepareTriggerRisingEdgeButton(dynamicData);
    prepareTriggerFallingEdgeButton(dynamicData);
    prepareTriggerSourceSelection(dynamicData);
}

void TriggerControls::prepareThresholdLabel(DynamicData &dynamicData)
{
    thresholdLabel = gtk_label_new("Threshold trigger value:");
    gtk_widget_set_hexpand(thresholdLabel, TRUE);
}

void TriggerControls::prepareTriggerThresholdSlider(DynamicData &dynamicData)
{
    // TODO: refactor to allow step=1mV instead of one y
    GtkAdjustment *adjustment = gtk_adjustment_new(
        DEFAULT_TRIGGER_THRESHOLD, 0, Y_DISPLAY_RESOLUTION, 1, 0.0, 0.0);
    triggerThresholdSlider =
        gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);

    gtk_widget_set_hexpand(triggerThresholdSlider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(triggerThresholdSlider), FALSE);

    g_signal_connect(triggerThresholdSlider, "value-changed",
                     G_CALLBACK(triggerThresholdSliderOnChangeAction),
                     &(dynamicData.triggerThresholdSliderValue));

    // TODO: refactor to display value in mV
    trigger_spin_button = gtk_spin_button_new(adjustment, 1.0, 0);
}

void TriggerControls::prepareTriggerHorizontalPositionLabel(
    DynamicData &dynamicData)
{
    triggerHorizontalPositionLabel =
        gtk_label_new("Trigger horizontal position:");
    gtk_widget_set_hexpand(thresholdLabel, TRUE);
}

void TriggerControls::prepareTriggerHorizontalPositionSlider(
    DynamicData &dynamicData)
{
    GtkAdjustment *adjustment = gtk_adjustment_new(
        (X_DISPLAY_RESOLUTION / 2), 0, X_DISPLAY_RESOLUTION, 1, 0.0, 0.0);
    triggerHorizontalPositionSlider =
        gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);

    gtk_widget_set_hexpand(triggerHorizontalPositionSlider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(triggerHorizontalPositionSlider), FALSE);

    g_signal_connect(triggerHorizontalPositionSlider, "value-changed",
                     G_CALLBACK(triggerHorizontalPositionSliderOnChangeAction),
                     &dynamicData);

    trigger_horizontal_position_spin_button =
        gtk_spin_button_new(adjustment, 1.0, 0);
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

void TriggerControls::prepareTriggerFallingEdgeButton(
    DynamicData &dynamicData)
{
    triggerFallingEdgeButton = gtk_button_new_with_label("‾‾\\__");
    g_signal_connect(triggerFallingEdgeButton, "clicked",
                     G_CALLBACK(onTriggerFallingEdgeButtonClicked),
                     &(dynamicData.thresholdTrigger));
}

void TriggerControls::prepareTriggerSourceSelection(DynamicData &dynamicData)
{
    trigger_source_label = gtk_label_new("Trigger source:");
    gtk_widget_set_hexpand(trigger_source_label, TRUE);

    trigger_source_combo = gtk_combo_box_text_new();

    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(trigger_source_combo), "ch1",
                              "Channel 1");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(trigger_source_combo), "ch2",
                              "Channel 2");

    gtk_combo_box_set_active(GTK_COMBO_BOX(trigger_source_combo),
                             DEFAULT_TRIGGER_SOURCE);

    g_signal_connect(trigger_source_combo, "changed",
                     G_CALLBACK(onTriggerSourceChanged),
                     &dynamicData.trigger_source);
}

GtkWidget *TriggerControls::getTriggerControlsContainer()
{
    GtkWidget *triggerControlsExpander = gtk_expander_new("Trigger controls");
    gtk_expander_set_expanded(GTK_EXPANDER(triggerControlsExpander), TRUE);

    GtkWidget *triggerControlsGrid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), thresholdLabel, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerThresholdSlider, 0, 1,
                    1, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), trigger_spin_button, 1, 1, 1,
                    1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerHorizontalPositionLabel,
                    0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid),
                    triggerHorizontalPositionSlider, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid),
                    trigger_horizontal_position_spin_button, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerLabel, 0, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerRisingEdgeButton, 0, 5,
                    1, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), triggerFallingEdgeButton, 1, 5,
                    1, 1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), trigger_source_label, 0, 6, 2,
                    1);
    gtk_grid_attach(GTK_GRID(triggerControlsGrid), trigger_source_combo, 0, 7, 2,
                    1);

    gtk_container_add(GTK_CONTAINER(triggerControlsExpander),
                      triggerControlsGrid);
    return triggerControlsExpander;
}
