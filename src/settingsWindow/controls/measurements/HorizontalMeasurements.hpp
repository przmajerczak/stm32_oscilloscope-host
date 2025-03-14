#pragma once

#include "sharedData/DynamicData.hpp"
#include <gtk/gtk.h>

class HorizontalMeasurements
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getHorizontalMeasurementsContainer();

private:
    GtkWidget *baseline_timestamp_slider = nullptr;
    GtkWidget *measured_timestamp_slider = nullptr;
    GtkWidget *baseline_timestamp_spin_button = nullptr;
    GtkWidget *measured_timestamp_spin_button = nullptr;

    GtkWidget *horizontalMeasurementsExpander = nullptr;
};
