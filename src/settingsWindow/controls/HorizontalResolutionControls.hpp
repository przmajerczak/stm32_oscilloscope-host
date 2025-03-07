#pragma once

#include "sharedData/DynamicData.hpp"
#include <gtk/gtk.h>

class HorizontalResolutionControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getHorizontalResolutionControlsContainer();

private:
    GtkWidget *horizontal_resolution_slider = nullptr;
    GtkWidget *horizontal_resolution_spin_button = nullptr;
};
