#pragma once

#include <gtk/gtk.h>

class DynamicData;

class AveragingControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getAveragingControlsContainer();

private:
    GtkWidget *averaging_window_size_spin_button = nullptr;
};
