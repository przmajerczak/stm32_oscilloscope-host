#pragma once

#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"
#include "display/utils/LineDrawer.hpp"
#include "DataAnalyzer.hpp"
#include "DataRetriever.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Oscilloscope
{
public:
    Oscilloscope(DynamicData &dynamicData);

    void run();
    void runDataRetrieve();

private:
    void display();
    void drawWaveform(const ChannelId channelId);
    DynamicData &dynamicData;
    LineDrawer lineDrawer{dynamicData};
    GLFWwindow *window = nullptr;

    DataAnalyzer dataAnalyzer;
    DataRetriever dataRetriever;
};
