#include "LineDrawer.hpp"

#include <GL/glew.h>

void LineDrawer::drawTriggerIndicator(const int x, const int y)
{
    glColor3f(COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY);

    drawVerticalLine(x, NEUTRAL);
    drawHorizontalLineWithLabels(y, "mV", textPrinterForTriggerIndicator,
                                 NEUTRAL);
}

void LineDrawer::drawGrid(const int numOfVerticalLayers,
                          const int numOfHorizontalLayers)
{
    glColor3f(COLOR_RGB_DARK_GRAY, COLOR_RGB_DARK_GRAY, COLOR_RGB_DARK_GRAY);

    drawHorizontalGrid(numOfHorizontalLayers);
    drawVerticalGrid(numOfVerticalLayers);
}

void LineDrawer::drawDisplayAreaBorder()
{
    glColor3f(COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY);

    constexpr float HALF_THICKNESS{VERY_BOLD / 2};

    drawVerticalLine(-1 * HALF_THICKNESS, VERY_BOLD);
    drawVerticalLine(X_DISPLAY_RESOLUTION + HALF_THICKNESS, VERY_BOLD);
    drawHorizontalLine(-1 * HALF_THICKNESS, VERY_BOLD);
    drawHorizontalLine(Y_DISPLAY_RESOLUTION + HALF_THICKNESS, VERY_BOLD);
}

void LineDrawer::drawHorizontalLine(const int y, const float boldness,
                                    const bool stipple_line)
{
    if (stipple_line)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
    }

    glLineWidth(boldness);

    glBegin(GL_LINES);

    glVertex2f(marginCorrected(0), marginCorrected(y));
    glVertex2f(marginCorrected(X_DISPLAY_RESOLUTION), marginCorrected(y));

    glEnd();

    if (stipple_line)
    {
        glDisable(GL_LINE_STIPPLE);
    }
}

void LineDrawer::drawVerticalLine(const int x, const float boldness,
                                  const bool stipple_line)
{

    if (stipple_line)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
    }

    glLineWidth(boldness);

    glBegin(GL_LINES);

    glVertex2f(marginCorrected(x), marginCorrected(0));
    glVertex2f(marginCorrected(x), marginCorrected(Y_DISPLAY_RESOLUTION));

    glEnd();

    if (stipple_line)
    {
        glDisable(GL_LINE_STIPPLE);
    }
}

void LineDrawer::drawHorizontalLineWithLabels(const int y,
                                              const char *unit_label,
                                              const TextPrinter &textPrinter,
                                              const float boldness_line,
                                              const bool stipple_line)
{
    drawHorizontalLine(y, boldness_line, stipple_line);

    const int right_x{marginCorrected(X_DISPLAY_RESOLUTION) +
                      static_cast<uint16_t>(2 * VERY_BOLD)};
    const int left_x{static_cast<uint16_t>(2 * VERY_BOLD)};
    const int upper_line_y{marginCorrected(y)};
    const int lower_line_y{upper_line_y - 20}; // TODO: remove magic number

    const std::string voltage_value{
        std::to_string(scaleYToVoltage_mV(globalData.verticalBoundsData, y))};

    textPrinter.drawText(right_x, upper_line_y, voltage_value.c_str());
    textPrinter.drawText(right_x, lower_line_y, unit_label);
    textPrinter.drawText(left_x, upper_line_y, voltage_value.c_str());
    textPrinter.drawText(left_x, lower_line_y, unit_label);
}

// TODO: move value_label generation inside
void LineDrawer::drawVerticalLineWithLabels(const int x,
                                            const char *value_label,
                                            const char *unit_label,
                                            const TextPrinter &textPrinter,
                                            const float boldness_line,
                                            const bool stipple_line)
{
    drawVerticalLine(x, boldness_line, stipple_line);

    const int upper_unit_y{marginCorrected(Y_DISPLAY_RESOLUTION) +
                           static_cast<uint16_t>(2 * VERY_BOLD)};
    const int upper_value_y{upper_unit_y + 20};
    const int lower_value_y{marginCorrected(0) -
                            static_cast<uint16_t>(2 * VERY_BOLD) - 20};
    const int lower_unit_y{lower_value_y - 20};

    textPrinter.drawText(marginCorrected(x) - 20, upper_value_y, value_label);
    textPrinter.drawText(marginCorrected(x) - 10, upper_unit_y, unit_label);
    textPrinter.drawText(marginCorrected(x) - 20, lower_value_y, value_label);
    textPrinter.drawText(marginCorrected(x) - 10, lower_unit_y, unit_label);
}

void LineDrawer::drawHorizontalGrid(const int numOfHorizontalLayers)
{
    const int middle_y{Y_DISPLAY_RESOLUTION / 2};

    drawHorizontalLineWithLabels(middle_y, " mV", textPrinterForGrid, NEUTRAL,
                                 true);
    drawHorizontalLineWithLabels(0, " mV", textPrinterForGrid, NEUTRAL, true);
    drawHorizontalLineWithLabels(Y_DISPLAY_RESOLUTION, " mV", textPrinterForGrid,
                                 NEUTRAL, true);

    const float delta_y{Y_DISPLAY_RESOLUTION / (2.0f * numOfHorizontalLayers)};
    for (int i = 1; i < numOfHorizontalLayers; ++i)
    {
        const int upper_y{middle_y + static_cast<int>(i * delta_y)};
        const int lower_y{middle_y - static_cast<int>(i * delta_y)};

        drawHorizontalLineWithLabels(upper_y, " mV", textPrinterForGrid, NEUTRAL,
                                     true);
        drawHorizontalLineWithLabels(lower_y, " mV", textPrinterForGrid, NEUTRAL,
                                     true);
    }
}

void LineDrawer::drawVerticalGrid(const int numOfVerticalLayers)
{
    const int middle_x{X_DISPLAY_RESOLUTION / 2};

    const float delta_x{static_cast<float>(X_DISPLAY_RESOLUTION) /
                        (2.0f * numOfVerticalLayers)};
    const double nanoseconds_per_x{globalData.horizontal_resolution_ns /
                                   static_cast<double>(X_DISPLAY_RESOLUTION)};

    double time_multiplier;
    const char *unit_label;

    if (globalData.horizontal_resolution_ns >= 2000000000)
    {
        time_multiplier = 0.000000001;
        unit_label = "s";
    }
    else if (globalData.horizontal_resolution_ns >= 2000000)
    {
        time_multiplier = 0.000001;
        unit_label = "ms";
    }
    else if (globalData.horizontal_resolution_ns >= 20000)
    {
        time_multiplier = 0.001;
        unit_label = "us";
    }
    else
    {
        time_multiplier = 1;
        unit_label = "ns";
    }

    drawVerticalLineWithLabels(middle_x, "0", unit_label, textPrinterForGrid,
                               BOLD, true);

    for (int i = 1; i <= numOfVerticalLayers - 1; ++i)
    {
        const int right_x{middle_x + static_cast<int>(i * delta_x)};
        const int left_x{middle_x - static_cast<int>(i * delta_x)};

        const double nanoseconds_for_line{(right_x - middle_x) * nanoseconds_per_x *
                                          time_multiplier};

        std::string left_time_value{
            doubleToFixedLengthString(-1.0 * nanoseconds_for_line, 4)};
        std::string right_time_value{
            doubleToFixedLengthString(nanoseconds_for_line, 4)};

        drawVerticalLineWithLabels(left_x, left_time_value.c_str(), unit_label,
                                   textPrinterForGrid, NEUTRAL, true);
        drawVerticalLineWithLabels(right_x, right_time_value.c_str(), unit_label,
                                   textPrinterForGrid, NEUTRAL, true);
    }
}
