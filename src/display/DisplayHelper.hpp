#pragma once

#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"
#include "utils/LineDrawer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DisplayHelper
{
public:
    DisplayHelper(const DynamicData &dynamicData);

    void run();

private:
    void display();
    void drawWaveform();
    const DynamicData &dynamicData;
    LineDrawer lineDrawer{dynamicData.verticalBoundsData};
    GLFWwindow *window = nullptr;
};
