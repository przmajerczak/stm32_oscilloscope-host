#include "HorizontalResolutionControls.hpp"

#include <cmath>

void horizontalResolutionSliderOnChangeAction(GtkRange *range,
                                              gpointer _dynamicData)
{
    DynamicData *dynamicData = (DynamicData *)_dynamicData;

    dynamicData->horizontal_resolution_ns = pow(10,
                                                gtk_range_get_value(range));
}

void HorizontalResolutionControls::prepare(DynamicData &dynamicData)
{
    // TODO: extract to constants.hpp
    // TODO set to 1-1000 and ad s/ms/us button
    constexpr uint8_t MIN_EXPONENT_OF_10{3};
    constexpr uint8_t DEFAULT_EXPONENT_OF_10{6};
    constexpr uint8_t MAX_EXPONENT_OF_10{8};
    constexpr double STEP{0.01};

    horizontal_resolution_slider = gtk_scale_new_with_range(
        GTK_ORIENTATION_HORIZONTAL, MIN_EXPONENT_OF_10, MAX_EXPONENT_OF_10, STEP);
    gtk_widget_set_hexpand(horizontal_resolution_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(horizontal_resolution_slider), TRUE);
    gtk_range_set_value(GTK_RANGE(horizontal_resolution_slider),
                        DEFAULT_EXPONENT_OF_10);

    g_signal_connect(horizontal_resolution_slider, "value-changed",
                     G_CALLBACK(horizontalResolutionSliderOnChangeAction),
                     &dynamicData);
};

GtkWidget *HorizontalResolutionControls::getHorizontalResolutionControlsContainer()
{
    GtkWidget *horizontalResolutionControlsExpander =
        gtk_expander_new("Horizontal resolution controls");
    gtk_expander_set_expanded(GTK_EXPANDER(horizontalResolutionControlsExpander),
                              TRUE);

    constexpr int spacing{0};
    GtkWidget *horizontalResolutionHorizontalBox =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);

    constexpr int padding{0};
    gtk_box_pack_start(GTK_BOX(horizontalResolutionHorizontalBox),
                       horizontal_resolution_slider, FALSE, TRUE, padding);

    gtk_container_add(GTK_CONTAINER(horizontalResolutionControlsExpander),
                      horizontalResolutionHorizontalBox);

    return horizontalResolutionControlsExpander;
};