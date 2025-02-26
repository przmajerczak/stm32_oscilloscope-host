#pragma once

#include "sharedData/DynamicData.hpp"
#include <gtk/gtk.h>

class HorizontalResolutionControls
{
public:
    void prepare(DynamicData &dynamicData) {};
    GtkWidget *getHorizontalResolutionControlsContainer()
    {
        GtkWidget *horizontalResolutionControlsExpander = gtk_expander_new("Horizontal resolution controls");
        gtk_expander_set_expanded(GTK_EXPANDER(horizontalResolutionControlsExpander), TRUE);

        return horizontalResolutionControlsExpander;
    };
};
