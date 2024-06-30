#pragma once

#include <gtk/gtk.h>
#include "types.hpp"

class SettingsWindow
{

public:
    static void run();
    static void notifyAboutThresholdTrigger();
    static uint16_t getTriggerThresholdSliderValue();
    static ThresholdTrigger getThresholdTrigger();

private:
    static void init();
    static gboolean frequencyLabelTimeoutAction(gpointer widgetPtr);
    static void triggerThresholdSliderOnChangeAction(GtkRange *range);
};
