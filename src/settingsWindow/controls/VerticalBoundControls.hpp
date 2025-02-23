#pragma once

#include <gtk/gtk.h>
#include "sharedData/DynamicData.hpp"

class VerticalBoundControls
{
public:
    static void prepare(DynamicData &dynamicData);
    static GtkWidget *getVerticalBoundControlsContainer();
    static float getVerticalLowerBoundValue();
    static float getVerticalUpperBoundValue();
    static float getVerticalLowerBoundValue_mV();
    static float getVerticalUpperBoundValue_mV();
};
