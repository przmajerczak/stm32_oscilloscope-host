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
                        dynamicData->verticalBoundsData.verticalUpperBound());

    dynamicData->verticalMeasurementsData.recalculateValues(*dynamicData);
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
                        dynamicData->verticalBoundsData.verticalLowerBound());

    dynamicData->verticalMeasurementsData.recalculateValues(*dynamicData);
}

void VerticalBoundControls::prepare(DynamicData &dynamicData)
{
    // TODO: make this display value in mV, not in ys
    GtkAdjustment *lower_bound_slider_adjustment = gtk_adjustment_new(
        INPUT_SIGNAL_MIN, INPUT_SIGNAL_MIN, INPUT_SIGNAL_MAX, 1, 0.0, 0.0);
    vertical_lower_bound_slider =
        gtk_scale_new(GTK_ORIENTATION_VERTICAL, lower_bound_slider_adjustment);
    gtk_widget_set_hexpand(vertical_lower_bound_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(vertical_lower_bound_slider), FALSE);
    gtk_range_set_inverted(GTK_RANGE(vertical_lower_bound_slider), TRUE);
    gtk_widget_set_size_request(vertical_lower_bound_slider, 0, DEFAULT_SLIDER_HEIGHT);

    GtkAdjustment *upper_bound_slider_adjustment = gtk_adjustment_new(
        INPUT_SIGNAL_MAX, INPUT_SIGNAL_MIN, INPUT_SIGNAL_MAX, 1, 0.0, 0.0);
    vertical_upper_bound_slider =
        gtk_scale_new(GTK_ORIENTATION_VERTICAL, upper_bound_slider_adjustment);
    gtk_widget_set_hexpand(vertical_upper_bound_slider, TRUE);
    gtk_scale_set_draw_value(GTK_SCALE(vertical_upper_bound_slider), FALSE);
    gtk_range_set_inverted(GTK_RANGE(vertical_upper_bound_slider), TRUE);
    gtk_widget_set_size_request(vertical_upper_bound_slider, 0, DEFAULT_SLIDER_HEIGHT);

    callbackDataForLowerBoundSlider.data = &dynamicData;
    callbackDataForLowerBoundSlider.widget = vertical_upper_bound_slider;

    g_signal_connect(vertical_lower_bound_slider, "value-changed",
                     G_CALLBACK(verticalLowerBoundSliderOnChangeAction),
                     &callbackDataForLowerBoundSlider);

    vertical_lower_bound_spin_button =
        gtk_spin_button_new(lower_bound_slider_adjustment, 1.0, 0);

    callbackDataForUpperBoundSlider.data = &dynamicData;
    callbackDataForUpperBoundSlider.widget = vertical_lower_bound_slider;

    g_signal_connect(vertical_upper_bound_slider, "value-changed",
                     G_CALLBACK(verticalUpperBoundSliderOnChangeAction),
                     &callbackDataForUpperBoundSlider);

    vertical_upper_bound_spin_button =
        gtk_spin_button_new(upper_bound_slider_adjustment, 1.0, 0);
}

GtkWidget *VerticalBoundControls::getVerticalBoundControlsContainer()
{
    GtkWidget *verticalBoundControlsExpander =
        gtk_expander_new("Vertical bound controls");
    gtk_expander_set_expanded(GTK_EXPANDER(verticalBoundControlsExpander), TRUE);

    constexpr int spacing{0};
    constexpr int padding{0};

    GtkWidget *vertical_lower_bound_controls_vertical_box =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    gtk_box_pack_start(GTK_BOX(vertical_lower_bound_controls_vertical_box),
                       vertical_lower_bound_spin_button, FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(vertical_lower_bound_controls_vertical_box),
                       vertical_lower_bound_slider, FALSE, TRUE, padding);

    GtkWidget *vertical_upper_bound_controls_vertical_box =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    gtk_box_pack_start(GTK_BOX(vertical_upper_bound_controls_vertical_box),
                       vertical_upper_bound_spin_button, FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(vertical_upper_bound_controls_vertical_box),
                       vertical_upper_bound_slider, FALSE, TRUE, padding);

    GtkWidget *verticalBoundControlsHorizontalBox =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);

    gtk_box_pack_start(GTK_BOX(verticalBoundControlsHorizontalBox),
                       vertical_lower_bound_controls_vertical_box, FALSE, TRUE,
                       padding);
    gtk_box_pack_start(GTK_BOX(verticalBoundControlsHorizontalBox),
                       vertical_upper_bound_controls_vertical_box, FALSE, TRUE,
                       padding);

    gtk_container_add(GTK_CONTAINER(verticalBoundControlsExpander),
                      verticalBoundControlsHorizontalBox);
    return verticalBoundControlsExpander;
}
