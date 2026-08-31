#pragma once

#include <gtk/gtk.h>

class DynamicData;

class HorizontalResolutionControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getHorizontalResolutionControlsContainer();

private:
    GtkWidget *horizontal_resolution_slider = nullptr;
};
