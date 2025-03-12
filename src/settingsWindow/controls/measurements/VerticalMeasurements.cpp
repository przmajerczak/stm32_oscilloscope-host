#include "VerticalMeasurements.hpp"
#include "sharedData/constants.hpp"

void baselineVoltageSliderOnChangeAction(GtkRange *range, gpointer data) {}

void measuredVoltageSliderOnChangeAction(GtkRange *range, gpointer data) {}

void VerticalMeasurements::prepare(DynamicData &dynamicData)
{
    GtkAdjustment *baseline_voltage_slider_adjustment = gtk_adjustment_new(
        Y_DISPLAY_RESOLUTION * 0.3, 0, Y_DISPLAY_RESOLUTION, 1, 0.0, 0.0);
    baseline_voltage_slider = gtk_scale_new(GTK_ORIENTATION_VERTICAL,
                                            baseline_voltage_slider_adjustment);
    gtk_widget_set_hexpand(baseline_voltage_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(baseline_voltage_slider), FALSE);
    gtk_range_set_inverted(GTK_RANGE(baseline_voltage_slider), TRUE);
    gtk_widget_set_size_request(baseline_voltage_slider, 0,
                                DEFAULT_SLIDER_HEIGHT);

    GtkAdjustment *measurement_voltage_slider_adjustment = gtk_adjustment_new(
        Y_DISPLAY_RESOLUTION * 0.6, 0, Y_DISPLAY_RESOLUTION, 1, 0.0, 0.0);
    measured_voltage_slider = gtk_scale_new(
        GTK_ORIENTATION_VERTICAL, measurement_voltage_slider_adjustment);
    gtk_widget_set_hexpand(measured_voltage_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(measured_voltage_slider), FALSE);
    gtk_range_set_inverted(GTK_RANGE(measured_voltage_slider), TRUE);
    gtk_widget_set_size_request(measured_voltage_slider, 0,
                                DEFAULT_SLIDER_HEIGHT);

    g_signal_connect(baseline_voltage_slider, "value-changed",
                     G_CALLBACK(baselineVoltageSliderOnChangeAction), nullptr);

    baseline_voltage_spin_button =
        gtk_spin_button_new(baseline_voltage_slider_adjustment, 1.0, 0);

    g_signal_connect(measured_voltage_slider, "value-changed",
                     G_CALLBACK(measuredVoltageSliderOnChangeAction), nullptr);

    measured_voltage_spin_button =
        gtk_spin_button_new(measurement_voltage_slider_adjustment, 1.0, 0);
}

GtkWidget *VerticalMeasurements::getVerticalMeasurementsContainer()
{
    GtkWidget *verticalMeasurementsExpander =
        gtk_expander_new("Vertical measurements");
    gtk_expander_set_expanded(GTK_EXPANDER(verticalMeasurementsExpander), FALSE);

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
