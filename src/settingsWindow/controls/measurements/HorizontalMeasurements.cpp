#include "HorizontalMeasurements.hpp"
#include "sharedData/constants.hpp"

void baselineTimestampSliderOnChangeAction(GtkRange *range, gpointer data)
{
    DynamicData *dynamicData = static_cast<DynamicData *>(data);
    dynamicData->horizontalMeasurementsData.notifyAboutBaselineChange(
        *dynamicData, static_cast<int16_t>(gtk_range_get_value(range)));
}

void measuredTimestampSliderOnChangeAction(
    GtkRange *range,
    gpointer
        data)
{
    DynamicData *dynamicData = static_cast<DynamicData *>(data);
    dynamicData->horizontalMeasurementsData.notifyAboutMeasurementChange(
        *dynamicData,
        static_cast<int16_t>(gtk_range_get_value(range)));
}

void horizontalMeasurementsExpenderNotifyAction(
    GtkExpander *expander, GParamSpec *pspec,
    gpointer data)
{
    DynamicData *dynamicData = static_cast<DynamicData *>(data);
    dynamicData->horizontalMeasurementsData.active = gtk_expander_get_expanded(expander);
}

void HorizontalMeasurements::prepare(DynamicData &dynamicData)
{
    GtkAdjustment *baseline_timestamp_slider_adjustment = gtk_adjustment_new(
        X_DISPLAY_RESOLUTION * 0.333, 0, X_DISPLAY_RESOLUTION, 1, 0.0, 0.0);
    baseline_timestamp_slider = gtk_scale_new(
        GTK_ORIENTATION_HORIZONTAL, baseline_timestamp_slider_adjustment);
    gtk_widget_set_hexpand(baseline_timestamp_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(baseline_timestamp_slider), FALSE);

    GtkAdjustment *measurement_timestamp_slider_adjustment = gtk_adjustment_new(
        X_DISPLAY_RESOLUTION * 0.666, 0, X_DISPLAY_RESOLUTION, 1, 0.0, 0.0);
    measured_timestamp_slider = gtk_scale_new(
        GTK_ORIENTATION_HORIZONTAL, measurement_timestamp_slider_adjustment);
    gtk_widget_set_hexpand(measured_timestamp_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(measured_timestamp_slider), FALSE);

    dynamicData.horizontalMeasurementsData.notifyAboutBaselineChange(
        dynamicData,
        static_cast<int16_t>(gtk_range_get_value(GTK_RANGE(baseline_timestamp_slider))));
    dynamicData.horizontalMeasurementsData.notifyAboutMeasurementChange(
        dynamicData,
        static_cast<int16_t>(gtk_range_get_value(GTK_RANGE(measured_timestamp_slider))));

    g_signal_connect(baseline_timestamp_slider, "value-changed",
                     G_CALLBACK(baselineTimestampSliderOnChangeAction),
                     &dynamicData);

    baseline_timestamp_spin_button =
        gtk_spin_button_new(baseline_timestamp_slider_adjustment, 1.0, 0);

    g_signal_connect(measured_timestamp_slider, "value-changed",
                     G_CALLBACK(measuredTimestampSliderOnChangeAction),
                     &dynamicData);

    measured_timestamp_spin_button =
        gtk_spin_button_new(measurement_timestamp_slider_adjustment, 1.0, 0);

    horizontalMeasurementsExpander = gtk_expander_new("Horizontal measurements");
    gtk_expander_set_expanded(GTK_EXPANDER(horizontalMeasurementsExpander),
                              FALSE);

    g_signal_connect(horizontalMeasurementsExpander, "notify::expanded",
                     G_CALLBACK(horizontalMeasurementsExpenderNotifyAction),
                     &dynamicData);
}

GtkWidget *HorizontalMeasurements::getHorizontalMeasurementsContainer()
{
    constexpr int spacing{0};
    constexpr int padding{0};

    GtkWidget *baseline_timestamp_controls_vertical_box =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    gtk_box_pack_start(GTK_BOX(baseline_timestamp_controls_vertical_box),
                       baseline_timestamp_spin_button, FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(baseline_timestamp_controls_vertical_box),
                       baseline_timestamp_slider, FALSE, TRUE, padding);

    GtkWidget *measured_timestamp_controls_vertical_box =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    gtk_box_pack_start(GTK_BOX(measured_timestamp_controls_vertical_box),
                       measured_timestamp_spin_button, FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(measured_timestamp_controls_vertical_box),
                       measured_timestamp_slider, FALSE, TRUE, padding);

    GtkWidget *horizontalMeasurementsHorizontalBox =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);

    gtk_box_pack_start(GTK_BOX(horizontalMeasurementsHorizontalBox),
                       baseline_timestamp_controls_vertical_box, FALSE, TRUE,
                       padding);
    gtk_box_pack_start(GTK_BOX(horizontalMeasurementsHorizontalBox),
                       measured_timestamp_controls_vertical_box, FALSE, TRUE,
                       padding);

    gtk_container_add(GTK_CONTAINER(horizontalMeasurementsExpander),
                      horizontalMeasurementsHorizontalBox);
    return horizontalMeasurementsExpander;
}
