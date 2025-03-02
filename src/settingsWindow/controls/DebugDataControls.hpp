#pragma once

#include <gtk/gtk.h>

#include "sharedData/DynamicData.hpp"

class DebugDataControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getDebugDataControlsContainer();
};
