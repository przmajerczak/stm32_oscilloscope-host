#pragma once

#include <gtk/gtk.h>

class VerticalBoundControls
{
public:
    static void prepare();
    static GtkWidget *getVerticalBoundControlsContainer();
    static float getVerticalLowerBoundValue();
    static float getVerticalUpperBoundValue();
    static float getVerticalLowerBoundValue_mV();
    static float getVerticalUpperBoundValue_mV();

private:
    static void verticalLowerBoundSliderOnChangeAction(GtkRange *range);
    static void verticalUpperBoundSliderOnChangeAction(GtkRange *range);
};
