#pragma once

#include "sharedData/ChannelData.hpp"
#include <gtk/gtk.h>

class AveragingControls
{
public:
    void prepare(ChannelData &channelData);
    GtkWidget *getAveragingControlsContainer();

private:
    GtkWidget *averaging_window_size_spin_button = nullptr;
};
