#pragma once

#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"
#include "utils/LineDrawer.hpp"
#include "DataAnalyzer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DisplayHelper
{
public:
    DisplayHelper(DynamicData &dynamicData);

    void run();

private:
    void display();
    void drawWaveform(const ChannelId channelId);
    DynamicData &dynamicData;
    LineDrawer lineDrawer{dynamicData};
    GLFWwindow *window = nullptr;

    DataAnalyzer dataAnalyzer;
};
