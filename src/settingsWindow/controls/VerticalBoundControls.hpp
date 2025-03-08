#pragma once

#include "settingsWindow/controls/CallbackData.hpp"
#include "sharedData/VerticalBoundsData.hpp"
#include <gtk/gtk.h>

class VerticalBoundControls
{
public:
    void prepare(VerticalBoundsData &verticalBoundsData);
    GtkWidget *getVerticalBoundControlsContainer();

private:
    GtkWidget *vertical_lower_bound_slider = nullptr;
    GtkWidget *vertical_upper_bound_slider = nullptr;
    GtkWidget *vertical_lower_bound_spin_button = nullptr;
    GtkWidget *vertical_upper_bound_spin_button = nullptr;

    CallbackData<VerticalBoundsData> callbackDataForLowerBoundSlider;
    CallbackData<VerticalBoundsData> callbackDataForUpperBoundSlider;
};
