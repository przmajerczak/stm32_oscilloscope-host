#pragma once

#include <gtk/gtk.h>
#include "types.hpp"

class SettingsWindow
{
public:
    static void run();
    static void notifyAboutThresholdTrigger();

    static ThresholdTrigger getThresholdTrigger();
    static uint16_t getTriggerThresholdY();

private:
    static void init();
    static gboolean frequencyLabelTimeoutAction(gpointer widgetPtr);
};
