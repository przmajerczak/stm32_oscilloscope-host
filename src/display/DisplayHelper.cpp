#include "DisplayHelper.hpp"
#include "debug/Timemarker.hpp"
#include "settingsWindow/controls/TriggerControls.hpp"
#include "sharedData/constants.hpp"
#include "utils.hpp"

DisplayHelper::DisplayHelper(DynamicData &dynamicData)
    : dynamicData(dynamicData)
{
    glfwInit();
    window = glfwCreateWindow(X_WINDOW_SIZE, Y_WINDOW_SIZE, "STM32 Oscilloscope",
                              nullptr, nullptr);

    glfwSetWindowPos(window, X_INITIAL_WINDOW_POSITION,
                     Y_INITIAL_WINDOW_POSITION);
    glfwMakeContextCurrent(window);

    auto framebuffer_size_callback = [](GLFWwindow *window, int width,
                                        int height)
    {
        glViewport(0, 0, width, height);
    };

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, X_WINDOW_SIZE, 0.0, Y_WINDOW_SIZE);
}

void DisplayHelper::display()
{
    Timemarker tmarker{dynamicData.globalData.timemarkersData
                           .customNonLibraryFrameDisplayDuration};

    glClear(GL_COLOR_BUFFER_BIT);

    lineDrawer.drawGrid(5, 4);

    drawWaveform();

    lineDrawer.drawTriggerIndicator(
        dynamicData.channelData.trigger_horizontal_position,
        dynamicData.channelData.triggerThresholdSliderValue);
    lineDrawer.drawDisplayAreaBorder();

    glFlush();
}

void DisplayHelper::drawWaveform()
{
    auto adcValuesToDisplay{
        dynamicData.channelData
            .adcValuesToDisplay}; // TODO: handle multithreading better
    auto value_it{adcValuesToDisplay.begin()};

    if (value_it == adcValuesToDisplay.end())
    {
        return;
    }

    glPointSize(1.0);
    glColor3f(1.0, 1.0, 0.0);
    glLineWidth(2.0);

    glBegin(GL_LINE_STRIP);

    float x{static_cast<float>(marginCorrected(0))};
    int y;

    const std::size_t values_to_display{adcValuesToDisplay.size()};
    const double x_length{static_cast<double>(X_DISPLAY_RESOLUTION) /
                          static_cast<double>(values_to_display)};

    while (*value_it == INVALID_VALUE)
    {
        x += x_length;
        ++value_it;
    }

    for (value_it; value_it != adcValuesToDisplay.end(); ++value_it)
    {
        if (*value_it == INVALID_VALUE)
        {
            break;
        }

        y = marginCorrected(scaleAdcValueToY(dynamicData, *value_it));
        glVertex2f(x, y);
        x += x_length;
    }
    glEnd();
}

void DisplayHelper::run()
{
    constexpr double fps{100.0};
    constexpr double frame_duration{1.0 / fps};
    double last_frame_time{0.0};

    while (!glfwWindowShouldClose(window))
    {
        const double current_time{glfwGetTime()};
        const bool time_for_new_frame{(current_time - last_frame_time) >=
                                      frame_duration};
        if (time_for_new_frame)
        {
            Timemarker tmarker{
                dynamicData.globalData.timemarkersData.totalFrameDisplayDuration};
            display();

            glfwSwapBuffers(window);
            glfwPollEvents();

            last_frame_time = current_time;
        }
    }
}
