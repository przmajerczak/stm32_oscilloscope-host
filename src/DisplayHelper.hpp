#pragma once

#include <GL/glut.h>

#include "drawingUtils.hpp"
#include "types.hpp"

class DisplayHelper
{
public:
    static void init(int argc, char **argv);
    static void run();

    static void triggerDisplay(const RawValuesContainer values_leftHalf,
                               const RawValuesContainer values_rightHalf);

private:
    static void display();
};
