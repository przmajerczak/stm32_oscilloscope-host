#include "VerticalMeasurements.hpp"
#include "sharedData/constants.hpp"

void baselineVoltageSliderOnChangeAction(GtkRange *range, gpointer data)
{
    DynamicData *dynamicData = static_cast<DynamicData *>(data);
    dynamicData->verticalMeasurementsData.notifyAboutBaselineChange(
        *dynamicData, static_cast<int16_t>(gtk_range_get_value(range)));
}

void measuredVoltageSliderOnChangeAction(GtkRange *range, gpointer data)
{
    DynamicData *dynamicData = static_cast<DynamicData *>(data);
    dynamicData->verticalMeasurementsData.notifyAboutMeasurementChange(
        *dynamicData, static_cast<int16_t>(gtk_range_get_value(range)));
}

void expenderNotifyAction(GtkExpander *expander, GParamSpec *pspec,
                          gpointer data)
{
    VerticalMeasurementsData *verticalMeasurementsData =
        static_cast<VerticalMeasurementsData *>(data);
    verticalMeasurementsData->active = gtk_expander_get_expanded(expander);
}

void VerticalMeasurements::prepare(DynamicData &dynamicData)
{
    GtkAdjustment *baseline_voltage_slider_adjustment = gtk_adjustment_new(
        Y_DISPLAY_RESOLUTION * 0.333, 0, Y_DISPLAY_RESOLUTION, 1, 0.0, 0.0);
    baseline_voltage_slider = gtk_scale_new(GTK_ORIENTATION_VERTICAL,
                                            baseline_voltage_slider_adjustment);
    gtk_widget_set_hexpand(baseline_voltage_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(baseline_voltage_slider), FALSE);
    gtk_range_set_inverted(GTK_RANGE(baseline_voltage_slider), TRUE);
    gtk_widget_set_size_request(baseline_voltage_slider, 0,
                                DEFAULT_SLIDER_HEIGHT);

    GtkAdjustment *measurement_voltage_slider_adjustment = gtk_adjustment_new(
        Y_DISPLAY_RESOLUTION * 0.666, 0, Y_DISPLAY_RESOLUTION, 1, 0.0, 0.0);
    measured_voltage_slider = gtk_scale_new(
        GTK_ORIENTATION_VERTICAL, measurement_voltage_slider_adjustment);
    gtk_widget_set_hexpand(measured_voltage_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(measured_voltage_slider), FALSE);
    gtk_range_set_inverted(GTK_RANGE(measured_voltage_slider), TRUE);
    gtk_widget_set_size_request(measured_voltage_slider, 0,
                                DEFAULT_SLIDER_HEIGHT);

    dynamicData.verticalMeasurementsData.notifyAboutBaselineChange(
        dynamicData, static_cast<int16_t>(gtk_range_get_value(GTK_RANGE(baseline_voltage_slider))));
    dynamicData.verticalMeasurementsData.notifyAboutMeasurementChange(
        dynamicData, static_cast<int16_t>(gtk_range_get_value(GTK_RANGE(measured_voltage_slider))));

    g_signal_connect(baseline_voltage_slider, "value-changed",
                     G_CALLBACK(baselineVoltageSliderOnChangeAction),
                     &dynamicData);

    baseline_voltage_spin_button =
        gtk_spin_button_new(baseline_voltage_slider_adjustment, 1.0, 0);

    g_signal_connect(measured_voltage_slider, "value-changed",
                     G_CALLBACK(measuredVoltageSliderOnChangeAction),
                     &dynamicData);

    measured_voltage_spin_button =
        gtk_spin_button_new(measurement_voltage_slider_adjustment, 1.0, 0);

    verticalMeasurementsExpander = gtk_expander_new("Vertical measurements");
    gtk_expander_set_expanded(GTK_EXPANDER(verticalMeasurementsExpander), FALSE);

    g_signal_connect(verticalMeasurementsExpander, "notify::expanded",
                     G_CALLBACK(expenderNotifyAction),
                     &dynamicData.verticalMeasurementsData);
}

GtkWidget *VerticalMeasurements::getVerticalMeasurementsContainer()
{
    constexpr int spacing{0};
    constexpr int padding{0};

    GtkWidget *baseline_voltage_controls_vertical_box =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    gtk_box_pack_start(GTK_BOX(baseline_voltage_controls_vertical_box),
                       baseline_voltage_spin_button, FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(baseline_voltage_controls_vertical_box),
                       baseline_voltage_slider, FALSE, TRUE, padding);

    GtkWidget *measured_voltage_controls_vertical_box =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    gtk_box_pack_start(GTK_BOX(measured_voltage_controls_vertical_box),
                       measured_voltage_spin_button, FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(measured_voltage_controls_vertical_box),
                       measured_voltage_slider, FALSE, TRUE, padding);

    GtkWidget *verticalMeasurementsHorizontalBox =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);

    gtk_box_pack_start(GTK_BOX(verticalMeasurementsHorizontalBox),
                       baseline_voltage_controls_vertical_box, FALSE, TRUE,
                       padding);
    gtk_box_pack_start(GTK_BOX(verticalMeasurementsHorizontalBox),
                       measured_voltage_controls_vertical_box, FALSE, TRUE,
                       padding);

    gtk_container_add(GTK_CONTAINER(verticalMeasurementsExpander),
                      verticalMeasurementsHorizontalBox);
    return verticalMeasurementsExpander;
}
