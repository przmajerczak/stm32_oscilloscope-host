#include "HorizontalResolutionControls.hpp"

void horizontalResolutionSliderOnChangeAction(GtkRange *range,
                                              gpointer _dynamicData)
{
    DynamicData *dynamicData = (DynamicData *)_dynamicData;

    dynamicData->horizontal_resolution_us =
        static_cast<uint32_t>(gtk_range_get_value(range));
}

void HorizontalResolutionControls::prepare(DynamicData &dynamicData)
{
    // TODO: extract to constants.hpp
    // TODO set to 1-1000 and ad s/ms/us button
    constexpr uint32_t TEMP_MIN_US{1000};
    constexpr uint32_t TEMP_MAX_US{200000};

    horizontal_resolution_slider = gtk_scale_new_with_range(
        GTK_ORIENTATION_HORIZONTAL, TEMP_MIN_US, TEMP_MAX_US, 1);
    gtk_widget_set_hexpand(horizontal_resolution_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(horizontal_resolution_slider), TRUE);
    gtk_range_set_value(GTK_RANGE(horizontal_resolution_slider),
                        DEFAULT_HORIZONTAL_RESOLUTION_US);

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