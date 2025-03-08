#pragma once

#include "sharedData/GlobalData.hpp"
#include <gtk/gtk.h>

class HorizontalResolutionControls
{
public:
    void prepare(GlobalData &globalData);
    GtkWidget *getHorizontalResolutionControlsContainer();

private:
    GtkWidget *horizontal_resolution_slider = nullptr;
    GtkWidget *horizontal_resolution_spin_button = nullptr;
};
