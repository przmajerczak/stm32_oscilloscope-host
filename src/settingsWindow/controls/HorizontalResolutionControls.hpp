#pragma once

#include "sharedData/DynamicData.hpp"
#include <gtk/gtk.h>

class HorizontalResolutionControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getHorizontalResolutionControlsContainer();
};
