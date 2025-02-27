#include "VerticalBoundControls.hpp"
#include "sharedData/constants.hpp"

void verticalLowerBoundSliderOnChangeAction(GtkRange *range,
                                            gpointer _callbackData)
{
    CallbackData<DynamicData> *callbackData =
        (CallbackData<DynamicData> *)_callbackData;
    GtkWidget *vertical_upper_bound_slider = callbackData->widget;
    DynamicData *dynamicData = callbackData->data;

    dynamicData->verticalBoundsData.notifyAboutLowerBoundChange(
        static_cast<uint16_t>(gtk_range_get_value(range)));

    gtk_range_set_value(GTK_RANGE(vertical_upper_bound_slider),
                        dynamicData->verticalBoundsData.vertical_upper_bound);
}

void verticalUpperBoundSliderOnChangeAction(GtkRange *range,
                                            gpointer _callbackData)
{
    CallbackData<DynamicData> *callbackData =
        (CallbackData<DynamicData> *)_callbackData;
    GtkWidget *vertical_lower_bound_slider = callbackData->widget;
    DynamicData *dynamicData = callbackData->data;

    dynamicData->verticalBoundsData.notifyAboutUpperBoundChange(
        static_cast<uint16_t>(gtk_range_get_value(range)));

    gtk_range_set_value(GTK_RANGE(vertical_lower_bound_slider),
                        dynamicData->verticalBoundsData.vertical_lower_bound);
}

void VerticalBoundControls::prepare(DynamicData &dynamicData)
{
    vertical_lower_bound_slider = gtk_scale_new_with_range(
        GTK_ORIENTATION_VERTICAL, INPUT_SIGNAL_MIN, INPUT_SIGNAL_MAX, 1);
    gtk_widget_set_hexpand(vertical_lower_bound_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(vertical_lower_bound_slider), TRUE);
    gtk_range_set_value(GTK_RANGE(vertical_lower_bound_slider), INPUT_SIGNAL_MIN);
    gtk_range_set_inverted(GTK_RANGE(vertical_lower_bound_slider), TRUE);
    gtk_widget_set_size_request(vertical_lower_bound_slider, 0, 300);

    vertical_upper_bound_slider = gtk_scale_new_with_range(
        GTK_ORIENTATION_VERTICAL, INPUT_SIGNAL_MIN, INPUT_SIGNAL_MAX, 1);
    gtk_widget_set_hexpand(vertical_upper_bound_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(vertical_upper_bound_slider), TRUE);
    gtk_range_set_value(GTK_RANGE(vertical_upper_bound_slider), INPUT_SIGNAL_MAX);
    gtk_range_set_inverted(GTK_RANGE(vertical_upper_bound_slider), TRUE);
    gtk_widget_set_size_request(vertical_upper_bound_slider, 0, 300);

    callbackDataForLowerBoundSlider.data = &dynamicData;
    callbackDataForLowerBoundSlider.widget = vertical_upper_bound_slider;

    g_signal_connect(vertical_lower_bound_slider, "value-changed",
                     G_CALLBACK(verticalLowerBoundSliderOnChangeAction),
                     &callbackDataForLowerBoundSlider);

    callbackDataForUpperBoundSlider.data = &dynamicData;
    callbackDataForUpperBoundSlider.widget = vertical_lower_bound_slider;

    g_signal_connect(vertical_upper_bound_slider, "value-changed",
                     G_CALLBACK(verticalUpperBoundSliderOnChangeAction),
                     &callbackDataForUpperBoundSlider);
}

GtkWidget *VerticalBoundControls::getVerticalBoundControlsContainer()
{
    GtkWidget *verticalBoundControlsExpander =
        gtk_expander_new("Vertical bound controls");
    gtk_expander_set_expanded(GTK_EXPANDER(verticalBoundControlsExpander), TRUE);

    constexpr int spacing{0};
    GtkWidget *verticalBoundControlsHorizontalBox =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);

    constexpr int padding{0};
    gtk_box_pack_start(GTK_BOX(verticalBoundControlsHorizontalBox),
                       vertical_lower_bound_slider, FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(verticalBoundControlsHorizontalBox),
                       vertical_upper_bound_slider, FALSE, TRUE, padding);

    gtk_container_add(GTK_CONTAINER(verticalBoundControlsExpander),
                      verticalBoundControlsHorizontalBox);
    return verticalBoundControlsExpander;
}
