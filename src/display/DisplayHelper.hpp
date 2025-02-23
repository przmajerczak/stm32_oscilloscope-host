#pragma once

#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"
#include "utils/LineDrawer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DisplayHelper
{
public:
    DisplayHelper();

    void run(const DynamicData &dynamicData);

private:
    void display(const DynamicData &dynamicData);
    void drawWaveform(const DynamicData &dynamicData);
    LineDrawer lineDrawer;
    GLFWwindow *window = nullptr;
};
