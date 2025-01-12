#pragma once

#include <gtk/gtk.h>

#include "CallbackData.hpp"
#include "sharedData/DynamicData.hpp"

class TemporaryFrequencyControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getFrequencyControlsContainer();

private:
    GtkWidget *frequencyLabel;
    CallbackData<DynamicData> callbackData;
};
