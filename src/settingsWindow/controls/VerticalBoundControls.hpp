#pragma once

#include "settingsWindow/controls/CallbackData.hpp"
#include <gtk/gtk.h>

class DynamicData;

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

    CallbackData<DynamicData> callbackDataForLowerBoundSlider;
    CallbackData<DynamicData> callbackDataForUpperBoundSlider;
};
