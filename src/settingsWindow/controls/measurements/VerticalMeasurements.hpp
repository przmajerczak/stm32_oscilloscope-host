#pragma once

#include <gtk/gtk.h>

class DynamicData;

class VerticalMeasurements
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getVerticalMeasurementsContainer();

private:
    GtkWidget *baseline_voltage_slider = nullptr;
    GtkWidget *measured_voltage_slider = nullptr;
    GtkWidget *baseline_voltage_spin_button = nullptr;
    GtkWidget *measured_voltage_spin_button = nullptr;

    GtkWidget *verticalMeasurementsExpander = nullptr;
};
