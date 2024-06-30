#pragma once

#include <gtk/gtk.h>

class SettingsWindow
{

public:
    static void run();
    static void notifyAboutThresholdTrigger();

private:
    static void init();
    static gboolean frequencyLabelTimeoutAction(gpointer widgetPtr);
};
