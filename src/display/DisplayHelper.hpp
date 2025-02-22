#pragma once

#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"
#include "utils/LineDrawer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DisplayHelper
{
public:
    void init();
    void run(DynamicData &dynamicData);

private:
    void display(DynamicData &dynamicData);
    LineDrawer lineDrawer;
    GLFWwindow *window = nullptr;
};
