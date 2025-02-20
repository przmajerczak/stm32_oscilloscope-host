#pragma once

#include <gtk/gtk.h>
#include "sharedData/types.hpp"

class TriggerControls
{
public:
    static void prepare();
    static GtkWidget* getTriggerControlsContainer();

    static uint16_t getTriggerThresholdY();
    static ThresholdTrigger getThresholdTrigger();
};
