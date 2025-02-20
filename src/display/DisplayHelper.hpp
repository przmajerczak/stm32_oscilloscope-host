#pragma once

#include "types.hpp"

class DisplayHelper
{
public:
    static void init(int argc, char **argv);
    static void run();

    static void triggerDisplay(const AdcValues &values);

private:
    static void display();
};
