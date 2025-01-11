#pragma once

#include <gtk/gtk.h>

#include "sharedData/DynamicData.hpp"

struct CallbackData
{
    GtkWidget *widget;
    DynamicData *dynamicData;
};

class TemporaryFrequencyControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getFrequencyControlsContainer();

private:
    GtkWidget *frequencyLabel;
    CallbackData callbackData;
};
