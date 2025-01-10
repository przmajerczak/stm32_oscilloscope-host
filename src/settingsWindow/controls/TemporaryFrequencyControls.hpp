#pragma once

#include <gtk/gtk.h>

class TemporaryFrequencyControls
{
public:
    static void prepare();
    static void notifyAboutThresholdTrigger();
    static GtkWidget *getFrequencyLabel();

private:
    static gboolean frequencyLabelTimeoutAction(gpointer widgetPtr);

    // TODO: get rid of it when class became non-static
    static uint32_t& getThresholdTriggersSinceLastFreqLabelReset();
};
