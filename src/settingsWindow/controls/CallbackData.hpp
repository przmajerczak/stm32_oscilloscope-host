#pragma once

#include <gtk/gtk.h>
#include "sharedData/DynamicData.hpp"

template <typename T>
struct CallbackData
{
    GtkWidget *widget;
    T *data;
};
