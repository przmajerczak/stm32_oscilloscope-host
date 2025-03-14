#include "LineDrawer.hpp"

#include "utils.hpp"
#include <GL/glew.h>
#include <cmath>
#include <string>

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

    constexpr double x1{-1 * HALF_THICKNESS};
    constexpr double x2{X_DISPLAY_RESOLUTION + HALF_THICKNESS};
    constexpr double y1{-1 * HALF_THICKNESS};
    constexpr double y2{Y_DISPLAY_RESOLUTION + HALF_THICKNESS};

    drawOutline(x1, x2, y1, y2, COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY,
                COLOR_RGB_LIGHT_GRAY, 1.0f, VERY_BOLD);
}

void LineDrawer::drawHorizontalLine(const int y, const float boldness,
                                    const bool stipple_line, const int x1,
                                    const int x2)
{
    if (stipple_line)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
    }

    glLineWidth(boldness);

    glBegin(GL_LINES);

    glVertex2f(marginCorrected(x1), marginCorrected(y));
    glVertex2f(marginCorrected(x2), marginCorrected(y));

    glEnd();

    if (stipple_line)
    {
        glDisable(GL_LINE_STIPPLE);
    }
}

void LineDrawer::drawVerticalLine(const int x, const float boldness,
                                  const bool stipple_line, const int y1,
                                  const int y2)
{
    if (stipple_line)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
    }

    glLineWidth(boldness);

    glBegin(GL_LINES);

    glVertex2f(marginCorrected(x), marginCorrected(y1));
    glVertex2f(marginCorrected(x), marginCorrected(y2));

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
        std::to_string(scaleYToVoltage_mV(dynamicData, y))};

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
    const int middle_x{static_cast<int>(dynamicData.trigger_horizontal_position)};

    const float delta_x{static_cast<float>(X_DISPLAY_RESOLUTION) /
                        (2.0f * numOfVerticalLayers)};

    double time_multiplier;
    const char *unit_label;

    if (dynamicData.horizontal_resolution_ns >= 1000000000)
    {
        time_multiplier = 0.000000001;
        unit_label = "s";
    }
    else if (dynamicData.horizontal_resolution_ns >= 1000000)
    {
        time_multiplier = 0.000001;
        unit_label = "ms";
    }
    else if (dynamicData.horizontal_resolution_ns >= 10000)
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

    for (int i = -2 * numOfVerticalLayers; i <= 2 * numOfVerticalLayers; ++i)
    {
        if (i == 0)
        {
            continue;
        }
        const int x{middle_x + static_cast<int>(i * delta_x)};

        // TODO: rethink and embed into loop contidtion
        if (x < 0)
        {
            continue;
        }

        if (x > X_DISPLAY_RESOLUTION)
        {
            break;
        }

        const double nanoseconds{scaleXToNanoseconds(dynamicData, x) * time_multiplier};
        std::string time_value{doubleToFixedLengthString(nanoseconds, 4)};
        drawVerticalLineWithLabels(x, time_value.c_str(), unit_label,
                                   textPrinterForGrid, NEUTRAL, true);
    }
}

void LineDrawer::drawVerticalMeasurement()
{
    const auto y_baseline{
        dynamicData.verticalMeasurementsData.baselineIndicator()};
    const auto y_measurement{
        dynamicData.verticalMeasurementsData.measurementIndicator()};

    drawRectangle(0, X_DISPLAY_RESOLUTION, y_baseline, y_measurement, 1.0f, 1.0f,
                  0.0f, 0.2f);

    constexpr float r{0.0f};
    constexpr float g{1.0f};
    constexpr float b{0.0f};

    glColor3f(r, g, b);
    drawHorizontalLine(y_baseline, BOLD);
    drawHorizontalLine(y_measurement, BOLD);

    const float left_label_x{2 * DISPLAY_MARGIN_WIDTH};
    const float right_label_x{X_DISPLAY_RESOLUTION - DISPLAY_MARGIN_WIDTH};

    drawVerticalLine(DISPLAY_MARGIN_WIDTH, NEUTRAL, true, y_baseline,
                     y_measurement);
    drawVerticalLine(X_DISPLAY_RESOLUTION - DISPLAY_MARGIN_WIDTH, NEUTRAL, true,
                     y_baseline, y_measurement);

    const float middle_y{
        (marginCorrected(y_measurement) + marginCorrected(y_baseline)) / 2};

    const auto vertical_difference{
        abs(dynamicData.verticalMeasurementsData.baselineIndicator_mV() -
            dynamicData.verticalMeasurementsData.measurementIndicator_mV())};

    const std::string baseline_label{
        std::to_string(static_cast<int16_t>(
            dynamicData.verticalMeasurementsData.baselineIndicator_mV())) +
        " mV"};
    const std::string difference_label{
        std::to_string(static_cast<int16_t>(vertical_difference)) + " mV"};
    const std::string measurement_label{
        std::to_string(static_cast<int16_t>(
            dynamicData.verticalMeasurementsData.measurementIndicator_mV())) +
        " mV"};

    textPrinterForVerticalMeasurement.drawText(
        left_label_x, marginCorrected(y_measurement), measurement_label.c_str(),
        true, r, g, b);
    textPrinterForVerticalMeasurement.drawText(
        left_label_x, middle_y, difference_label.c_str(), true, r, g, b);
    textPrinterForVerticalMeasurement.drawText(
        left_label_x, marginCorrected(y_baseline), baseline_label.c_str(), true,
        r, g, b);

    textPrinterForVerticalMeasurement.drawText(
        right_label_x, marginCorrected(y_measurement), measurement_label.c_str(),
        true, r, g, b);
    textPrinterForVerticalMeasurement.drawText(
        right_label_x, middle_y, difference_label.c_str(), true, r, g, b);
    textPrinterForVerticalMeasurement.drawText(
        right_label_x, marginCorrected(y_baseline), baseline_label.c_str(), true,
        r, g, b);
}

void LineDrawer::drawHorizontalMeasurement()
{
    const auto x_baseline{
        dynamicData.horizontalMeasurementsData.baselineIndicator()};
    const auto x_measurement{
        dynamicData.horizontalMeasurementsData.measurementIndicator()};

    drawRectangle(x_baseline, x_measurement, 0, Y_DISPLAY_RESOLUTION, 1.0f, 1.0f,
                  0.0f, 0.2f);

    constexpr float r{1.0f};
    constexpr float g{0.0f};
    constexpr float b{0.0f};

    glColor3f(r, g, b);
    drawVerticalLine(x_baseline, BOLD);
    drawVerticalLine(x_measurement, BOLD);
}
