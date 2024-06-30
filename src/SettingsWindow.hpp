#pragma once

#include <gtk/gtk.h>

class SettingsWindow
{

public:
    static void run();
    static void notifyAboutThresholdTrigger();
    static uint16_t getTriggerThresholdSliderValue();

private:
    static void init();
    static gboolean frequencyLabelTimeoutAction(gpointer widgetPtr);
};
