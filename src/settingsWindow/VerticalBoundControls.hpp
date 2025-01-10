#pragma once

#include <gtk/gtk.h>
#include "constants.hpp"

namespace
{
    GtkWidget *vertical_lower_bound_slider = nullptr;
    GtkWidget *vertical_upper_bound_slider = nullptr;

    float vertical_lower_bound{INPUT_SIGNAL_MIN};
    float vertical_upper_bound{INPUT_SIGNAL_MAX};
} // namespace

class VerticalBoundControls
{
public:
    static void prepare()
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

    static GtkWidget *verticalLowerBoundSlider()
    {
        return vertical_lower_bound_slider;
    }

    static GtkWidget *verticalUpperBoundSlider()
    {
        return vertical_upper_bound_slider;
    }

    static float getVerticalLowerBoundValue()
    {
        return vertical_lower_bound;
    }

    static float getVerticalUpperBoundValue()
    {
        return vertical_upper_bound;
    }

private:
    static void verticalLowerBoundSliderOnChangeAction(GtkRange *range)
    {
        vertical_lower_bound = static_cast<uint16_t>(gtk_range_get_value(range));

        if (vertical_lower_bound > vertical_upper_bound)
        {
            vertical_upper_bound = vertical_lower_bound;
            gtk_range_set_value(GTK_RANGE(vertical_upper_bound_slider),
                                vertical_upper_bound);
        }
    }

    static void verticalUpperBoundSliderOnChangeAction(GtkRange *range)
    {
        vertical_upper_bound = static_cast<uint16_t>(gtk_range_get_value(range));

        if (vertical_upper_bound < vertical_lower_bound)
        {
            vertical_lower_bound = vertical_upper_bound;
            gtk_range_set_value(GTK_RANGE(vertical_lower_bound_slider),
                                vertical_lower_bound);
        }
    }
};
