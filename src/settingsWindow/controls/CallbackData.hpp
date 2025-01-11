#pragma once

#include <gtk/gtk.h>
#include "sharedData/DynamicData.hpp"

struct CallbackData
{
    GtkWidget *widget;
    DynamicData *dynamicData;
};
