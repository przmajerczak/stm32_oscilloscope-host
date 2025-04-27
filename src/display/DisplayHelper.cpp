#include "DisplayHelper.hpp"

#include "debug/Timemarker.hpp"
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
    Timemarker tmarker{
        dynamicData.timemarkersData.customNonLibraryFrameDisplayDuration};

    glClear(GL_COLOR_BUFFER_BIT);

    lineDrawer.drawGrid(5, 4);

    for (ChannelId chId = CHANNEL_1; chId < NUMBER_OF_CHANNELS; ++chId)
    {
        if (dynamicData.active_channels.at(chId))
        {
            drawWaveform(chId);
        }
    }

    lineDrawer.drawTriggerIndicator(dynamicData.trigger_horizontal_position,
                                    dynamicData.triggerThresholdSliderValue);
    lineDrawer.drawDisplayAreaBorder();

    if (dynamicData.verticalMeasurementsData.active)
    {
        lineDrawer.drawVerticalMeasurement();
    }

    if (dynamicData.horizontalMeasurementsData.active)
    {
        lineDrawer.drawHorizontalMeasurement();
    }

    glFlush();
}

void DisplayHelper::drawWaveform(const ChannelId channelId)
{
    if (dynamicData.retrieved_adc_values.at(channelId).empty())
    {
        return;
    }

    // TODO: handle multithreading better
    auto adcValuesToDisplay{dataAnalyzer.prepareData(
        dynamicData.retrieved_adc_values.at(channelId), dynamicData, channelId)};

    auto value_it{adcValuesToDisplay.begin()};

    if (value_it == adcValuesToDisplay.end())
    {
        return;
    }

    glPointSize(1.0);
    glColor4fv(CHANNELS_COLORS.at(channelId));
    glLineWidth(2.0);

    glBegin(GL_LINE_STRIP);

    float x;
    float y;

    const double x_length{
        (static_cast<double>(X_DISPLAY_RESOLUTION) *
         dynamicData.nanoseconds_per_sample) /
        static_cast<double>(dynamicData.horizontal_resolution_ns)};

    if (dynamicData.trigger_index == INVALID_VALUE)
    {
        x = marginCorrected(dynamicData.trigger_horizontal_position);
    }
    else
    {
        x = marginCorrected(dynamicData.trigger_horizontal_position -
                            (dynamicData.trigger_index * x_length));
    }

    if (x < marginCorrected(0.0f))
    {
        const std::size_t shiftToDisplayBeginning{
            static_cast<std::size_t>((marginCorrected(0.0f) - x) / x_length)};
        x += shiftToDisplayBeginning * x_length;
        value_it += shiftToDisplayBeginning;
    }

    for (value_it; value_it != adcValuesToDisplay.end(); ++value_it)
    {
        if (x > marginCorrected(X_DISPLAY_RESOLUTION))
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
        if (dynamicData.new_data_available or
            dynamicData.display_configuration_changed)
        {
            const double current_time{glfwGetTime()};
            const bool time_for_new_frame{(current_time - last_frame_time) >=
                                          frame_duration};
            if (time_for_new_frame)
            {
                Timemarker tmarker{
                    dynamicData.timemarkersData.totalFrameDisplayDuration};

                display();

                glfwSwapBuffers(window);
                glfwPollEvents();

                last_frame_time = current_time;

                dynamicData.new_data_available = false;
                dynamicData.display_configuration_changed = false;
            }
        }
    }
}
