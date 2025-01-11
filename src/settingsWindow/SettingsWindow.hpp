#pragma once

#include <gtk/gtk.h>

#include "controls/TemporaryFrequencyControls.hpp"

class SettingsWindow
{
public:
    void runAsSeparateThread();

private:
    void configure();
    void fill();

    GtkWidget *window;

    TemporaryFrequencyControls temporaryFrequencyControls;
};
