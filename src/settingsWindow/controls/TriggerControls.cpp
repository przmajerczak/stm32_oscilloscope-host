#include "TriggerControls.hpp"

#include "sharedData/constants.hpp"
#include "sharedData/types.hpp"

void triggerThresholdSliderOnChangeAction(GtkRange *range, gpointer data)
{
    // TODO: use static_cast instead of C-style casts
    DynamicData *dynamicData = (DynamicData *)data;
    dynamicData->triggerThresholdSliderValue =
        static_cast<uint16_t>(gtk_range_get_value(range));
}

void triggerHorizontalPositionSliderOnChangeAction(GtkRange *range,
                                                   gpointer data)
{
    DynamicData *dynamicData = (DynamicData *)data;
    dynamicData->trigger_horizontal_position =
        static_cast<uint16_t>(gtk_range_get_value(range));
    dynamicData->horizontalMeasurementsData.recalculateValues(*dynamicData);
}

void onTriggerRisingEdgeButtonClicked(GtkWidget *button, gpointer data)
{
    DynamicData *dynamicData = (DynamicData *)data;
    dynamicData->thresholdTrigger = ThresholdTrigger::RISING_EDGE;
}

void onTriggerFallingEdgeButtonClicked(GtkWidget *button, gpointer data)
{
    DynamicData *dynamicData = (DynamicData *)data;
    dynamicData->thresholdTrigger = ThresholdTrigger::FALLING_EDGE;
}

static void onTriggerSourceChanged(GtkComboBox *widget, gpointer data)
{
    DynamicData *dynamicData = (DynamicData *)data;
    dynamicData->trigger_source = gtk_combo_box_get_active(widget);
}

void TriggerControls::prepare(DynamicData &dynamicData)
{
    prepareTriggerThresholdControls(dynamicData);
    prepareTriggerHorizontalPositionControls(dynamicData);
    prepareTriggerEdgeControls(dynamicData);
    prepareTriggerSourceSelection(dynamicData);
}

void TriggerControls::prepareTriggerThresholdControls(DynamicData &dynamicData)
{
    trigger_threshold_label = gtk_label_new("Trigger threshold value:");
    gtk_widget_set_hexpand(trigger_threshold_label, TRUE);

    // TODO: refactor to allow step=1mV instead of one y
    GtkAdjustment *adjustment = gtk_adjustment_new(
        DEFAULT_TRIGGER_THRESHOLD, 0, Y_DISPLAY_RESOLUTION, 1, 0.0, 0.0);
    trigger_threshold_slider =
        gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);

    gtk_widget_set_hexpand(trigger_threshold_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(trigger_threshold_slider), FALSE);

    g_signal_connect(trigger_threshold_slider, "value-changed",
                     G_CALLBACK(triggerThresholdSliderOnChangeAction),
                     &dynamicData);

    // TODO: refactor to display value in mV
    trigger_spin_button = gtk_spin_button_new(adjustment, 1.0, 0);
}

void TriggerControls::prepareTriggerHorizontalPositionControls(
    DynamicData &dynamicData)
{
    trigger_horizontal_position_label =
        gtk_label_new("Trigger horizontal position:");
    gtk_widget_set_hexpand(trigger_threshold_label, TRUE);

    GtkAdjustment *adjustment = gtk_adjustment_new(
        (X_DISPLAY_RESOLUTION / 2), 0, X_DISPLAY_RESOLUTION, 1, 0.0, 0.0);
    trigger_horizontal_position_slider =
        gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);

    gtk_widget_set_hexpand(trigger_horizontal_position_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(trigger_horizontal_position_slider), FALSE);

    g_signal_connect(trigger_horizontal_position_slider, "value-changed",
                     G_CALLBACK(triggerHorizontalPositionSliderOnChangeAction),
                     &dynamicData);

    trigger_horizontal_position_spin_button =
        gtk_spin_button_new(adjustment, 1.0, 0);
}

void TriggerControls::prepareTriggerEdgeControls(DynamicData &dynamicData)
{
    trigger_edge_label = gtk_label_new("Trigger edge:");
    gtk_widget_set_hexpand(trigger_edge_label, TRUE);

    trigger_rising_edge_button = gtk_button_new_with_label("__/‾‾");
    gtk_widget_set_hexpand(trigger_rising_edge_button, TRUE);
    g_signal_connect(trigger_rising_edge_button, "clicked",
                     G_CALLBACK(onTriggerRisingEdgeButtonClicked),
                     &dynamicData);

    trigger_falling_edge_button = gtk_button_new_with_label("‾‾\\__");
    gtk_widget_set_hexpand(trigger_falling_edge_button, TRUE);
    g_signal_connect(trigger_falling_edge_button, "clicked",
                     G_CALLBACK(onTriggerFallingEdgeButtonClicked),
                     &dynamicData);
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
                     &dynamicData);
}

GtkWidget *TriggerControls::getTriggerControlsContainer()
{
    GtkWidget *triggerControlsExpander = gtk_expander_new("Trigger controls");
    gtk_expander_set_expanded(GTK_EXPANDER(triggerControlsExpander), TRUE);

    constexpr int spacing{0};
    constexpr int padding{0};

    GtkWidget *trigger_threshold_box =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);
    gtk_box_pack_start(GTK_BOX(trigger_threshold_box), trigger_threshold_slider,
                       FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(trigger_threshold_box), trigger_spin_button, FALSE,
                       TRUE, padding);

    GtkWidget *trigger_horizontal_position_box =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);
    gtk_box_pack_start(GTK_BOX(trigger_horizontal_position_box),
                       trigger_horizontal_position_slider, FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(trigger_horizontal_position_box),
                       trigger_horizontal_position_spin_button, FALSE, TRUE,
                       padding);

    GtkWidget *trigger_edge_box =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);
    gtk_box_pack_start(GTK_BOX(trigger_edge_box), trigger_rising_edge_button, FALSE,
                       TRUE, padding);
    gtk_box_pack_start(GTK_BOX(trigger_edge_box), trigger_falling_edge_button, FALSE,
                       TRUE, padding);

    GtkWidget *trigger_controls_box =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    gtk_box_pack_start(GTK_BOX(trigger_controls_box), trigger_threshold_label, FALSE, TRUE,
                       padding);
    gtk_box_pack_start(GTK_BOX(trigger_controls_box), trigger_threshold_box,
                       FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(trigger_controls_box),
                       trigger_horizontal_position_label, FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(trigger_controls_box),
                       trigger_horizontal_position_box, FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(trigger_controls_box), trigger_edge_label, FALSE, TRUE,
                       padding);
    gtk_box_pack_start(GTK_BOX(trigger_controls_box), trigger_edge_box, FALSE,
                       TRUE, padding);
    gtk_box_pack_start(GTK_BOX(trigger_controls_box), trigger_source_label, FALSE,
                       TRUE, padding);
    gtk_box_pack_start(GTK_BOX(trigger_controls_box), trigger_source_combo, FALSE,
                       TRUE, padding);

    gtk_container_add(GTK_CONTAINER(triggerControlsExpander),
                      trigger_controls_box);
    return triggerControlsExpander;
}
