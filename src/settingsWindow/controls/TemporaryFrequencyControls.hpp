#pragma once

#include <gtk/gtk.h>

struct CallbackData
{
    GtkWidget *widget;
};

class TemporaryFrequencyControls
{
public:
    void prepare();
    GtkWidget *getFrequencyControlsContainer();

private:
    GtkWidget *frequencyLabel;
    CallbackData callbackData;
};
