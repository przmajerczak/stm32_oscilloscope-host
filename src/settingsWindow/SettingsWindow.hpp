#pragma once

#include <gtk/gtk.h>
#include "types.hpp"

class SettingsWindow
{
public:
    static void runAsSeparateThread();

private:
    static void configure();
    static void fill();
};
