#pragma once

#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"
#include "utils/LineDrawer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DisplayHelper
{
public:
    DisplayHelper(DynamicData &dynamicData);

    void run();

private:
    void display();
    void drawWaveform();
    DynamicData &dynamicData;
    LineDrawer lineDrawer{dynamicData.globalData};
    GLFWwindow *window = nullptr;
};
