#pragma once

#include <gtk/gtk.h>

class TemporaryFrequencyControls
{
public:
    void prepare();
    GtkWidget *getFrequencyControlsContainer();

private:
    GtkWidget *frequencyLabel;
};
