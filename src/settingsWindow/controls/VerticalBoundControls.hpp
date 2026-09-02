#pragma once

#include <gtk/gtk.h>

#include "settingsWindow/controls/CallbackData.hpp"

class DynamicData;

struct ButtonsCallbackData
{
    DynamicData *dynamicData;
    GtkWidget *upper_slider;
    GtkWidget *lower_slider;
};

class VerticalBoundControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getVerticalBoundControlsContainer();

private:
    GtkWidget *vertical_lower_bound_slider = nullptr;
    GtkWidget *vertical_upper_bound_slider = nullptr;
    GtkWidget *vertical_lower_bound_spin_button = nullptr;
    GtkWidget *vertical_upper_bound_spin_button = nullptr;
    GtkWidget *autoselect_button = nullptr;
    GtkWidget *zoomout_button = nullptr;

    ButtonsCallbackData buttons_callback_data;
    CallbackData<DynamicData> callbackDataForLowerBoundSlider;
    CallbackData<DynamicData> callbackDataForUpperBoundSlider;
};
