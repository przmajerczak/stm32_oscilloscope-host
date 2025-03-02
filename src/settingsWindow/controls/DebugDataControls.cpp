#include "DebugDataControls.hpp"

void DebugDataControls::prepare(DynamicData &dynamicData)
{
}

GtkWidget *DebugDataControls::getDebugDataControlsContainer()
{
    GtkWidget *debugDataControlsExpander = gtk_expander_new("Debug");
    gtk_expander_set_expanded(GTK_EXPANDER(debugDataControlsExpander), FALSE);

    constexpr int spacing{0};
    GtkWidget *measurementsVerticalBox =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    constexpr int padding{0};

    gtk_container_add(GTK_CONTAINER(debugDataControlsExpander),
                      measurementsVerticalBox);
    return debugDataControlsExpander;
}
