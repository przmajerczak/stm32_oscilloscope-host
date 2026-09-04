#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "DataAnalyzer.hpp"
#include "dataAcquisition/DataRetriever.hpp"
#include "display/LineDrawer.hpp"
#include "sharedData/DynamicData.hpp"
#include "sharedData/types.hpp"

class Oscilloscope
{
public:
    Oscilloscope(DynamicData &dynamicData, const bool test_mode);

    void run();
    void runDataRetrieve();

private:
    void display();
    void drawWaveform(const ChannelId channelId);
    float scaleMillivoltsToYWithinBounds(const DynamicData &dynamicData,
                                         const float millivolts) const;

    DynamicData &dynamicData;
    LineDrawer lineDrawer{dynamicData};
    GLFWwindow *window = nullptr;

    DataAnalyzer dataAnalyzer;
    DataRetriever dataRetriever;
};
