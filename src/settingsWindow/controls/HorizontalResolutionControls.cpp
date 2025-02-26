#include "HorizontalResolutionControls.hpp"

void HorizontalResolutionControls::prepare(DynamicData &dynamicData) {};

GtkWidget *HorizontalResolutionControls::getHorizontalResolutionControlsContainer()
{
    GtkWidget *horizontalResolutionControlsExpander = gtk_expander_new("Horizontal resolution controls");
    gtk_expander_set_expanded(GTK_EXPANDER(horizontalResolutionControlsExpander), TRUE);

    return horizontalResolutionControlsExpander;
};