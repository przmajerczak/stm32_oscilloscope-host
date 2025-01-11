#include "VerticalBoundControls.hpp"
#include "constants.hpp"

namespace
{
    GtkWidget *vertical_lower_bound_slider = nullptr;
    GtkWidget *vertical_upper_bound_slider = nullptr;

    float vertical_lower_bound{INPUT_SIGNAL_MIN};
    float vertical_upper_bound{INPUT_SIGNAL_MAX};
} // namespace

// TODO: move back to utils.hpp
float scaleAdcTo_mV(const uint16_t adc_value)
{
    return static_cast<float>(
        (ABSOULTE_VERTICAL_RESOLUTION_mV * static_cast<float>(adc_value) +
         MIN_VOLTAGE_mV) /
        static_cast<float>(INPUT_SIGNAL_MAX));
}

void VerticalBoundControls::prepare()
{
    vertical_lower_bound_slider = gtk_scale_new_with_range(
        GTK_ORIENTATION_HORIZONTAL, INPUT_SIGNAL_MIN, INPUT_SIGNAL_MAX, 1);
    gtk_widget_set_hexpand(vertical_lower_bound_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(vertical_lower_bound_slider), TRUE);
    gtk_range_set_value(GTK_RANGE(vertical_lower_bound_slider), INPUT_SIGNAL_MIN);

    g_signal_connect(vertical_lower_bound_slider, "value-changed",
                     G_CALLBACK(verticalLowerBoundSliderOnChangeAction), nullptr);

    vertical_upper_bound_slider = gtk_scale_new_with_range(
        GTK_ORIENTATION_HORIZONTAL, INPUT_SIGNAL_MIN, INPUT_SIGNAL_MAX, 1);
    gtk_widget_set_hexpand(vertical_upper_bound_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(vertical_upper_bound_slider), TRUE);
    gtk_range_set_value(GTK_RANGE(vertical_upper_bound_slider), INPUT_SIGNAL_MAX);

    g_signal_connect(vertical_upper_bound_slider, "value-changed",
                     G_CALLBACK(verticalUpperBoundSliderOnChangeAction), nullptr);
}

GtkWidget *VerticalBoundControls::getVerticalBoundControlsContainer()
{
    GtkWidget *verticalBoundControlsGrid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(verticalBoundControlsGrid), vertical_lower_bound_slider, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(verticalBoundControlsGrid), vertical_upper_bound_slider, 0, 1, 1, 1);

    return verticalBoundControlsGrid;
}

float VerticalBoundControls::getVerticalLowerBoundValue()
{
    return vertical_lower_bound;
}

float VerticalBoundControls::getVerticalUpperBoundValue()
{
    return vertical_upper_bound;
}

float VerticalBoundControls::getVerticalLowerBoundValue_mV()
{
    return scaleAdcTo_mV(vertical_lower_bound);
}

float VerticalBoundControls::getVerticalUpperBoundValue_mV()
{
    return scaleAdcTo_mV(vertical_upper_bound);
}

void VerticalBoundControls::verticalLowerBoundSliderOnChangeAction(GtkRange *range)
{
    vertical_lower_bound = static_cast<uint16_t>(gtk_range_get_value(range));

    if (vertical_lower_bound > vertical_upper_bound)
    {
        vertical_upper_bound = vertical_lower_bound;
        gtk_range_set_value(GTK_RANGE(vertical_upper_bound_slider),
                            vertical_upper_bound);
    }
}

void VerticalBoundControls::verticalUpperBoundSliderOnChangeAction(GtkRange *range)
{
    vertical_upper_bound = static_cast<uint16_t>(gtk_range_get_value(range));

    if (vertical_upper_bound < vertical_lower_bound)
    {
        vertical_lower_bound = vertical_upper_bound;
        gtk_range_set_value(GTK_RANGE(vertical_lower_bound_slider),
                            vertical_lower_bound);
    }
}
