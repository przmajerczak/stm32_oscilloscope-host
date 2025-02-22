#pragma once

#include <gtk/gtk.h>

#include "controls/TemporaryFrequencyControls.hpp"
#include "controls/TriggerControls.hpp"
#include "sharedData/DynamicData.hpp"

class SettingsWindow
{
public:
    void runAsSeparateThread(DynamicData &dynamicData);

private:
    void configure();
    void fill(DynamicData &dynamicData);

    GtkWidget *window;

    TemporaryFrequencyControls temporaryFrequencyControls;
    TriggerControls triggerControls;
};
