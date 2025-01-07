#pragma once

#include "constants.hpp"
#include "utils.hpp"
#include <GL/glut.h>
#include <string>

static void drawText(const float x, const float y, const char *text,
                     const float font_size, const float color_rgb,
                     const float boldness = NEUTRAL)
{
    if (text != nullptr)
    {
        glPushMatrix();
        glTranslatef(x, y, 0);
        glColor3f(color_rgb, color_rgb, color_rgb);
        glLineWidth(boldness);
        glScalef(font_size, font_size, font_size);
        for (const char *character = text; (*character) != 0; ++character)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, (*character));
        }
        glPopMatrix();
    }
}

static void drawHorizontalLine(const int y, const float boldness, const bool stipple_line = false)
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

static void drawVerticalLine(const int x, const float boldness, const bool stipple_line = false)
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

static void drawHorizontalLineWithLabels(const int y, const char *value_label, const char *unit_label,
                                         const float font_size, const float color_rgb,
                                         const float boldness_line = NEUTRAL, const float boldness_text = NEUTRAL,
                                         const bool stipple_line = false)
{

    drawHorizontalLine(y, boldness_line, stipple_line);

    const int right_x{marginCorrected(X_DISPLAY_RESOLUTION) +
                      static_cast<uint16_t>(2 * VERY_BOLD)};
    const int left_x{static_cast<uint16_t>(2 * VERY_BOLD)};
    const int upper_line_y{marginCorrected(y)};
    const int lower_line_y{upper_line_y - 20}; // TODO: remove magic number

    drawText(right_x, upper_line_y, value_label, font_size, color_rgb, boldness_text);
    drawText(right_x, lower_line_y, unit_label, font_size, color_rgb, boldness_text);
    drawText(left_x, upper_line_y, value_label, font_size, color_rgb, boldness_text);
    drawText(left_x, lower_line_y, unit_label, font_size, color_rgb, boldness_text);
}

static void drawVerticalLineWithLabels(const int x, const char *value_label, const char *unit_label,
                                       const float font_size, const float color_rgb,
                                       const float boldness_line = NEUTRAL, const float boldness_text = NEUTRAL,
                                       const bool stipple_line = false)
{

    drawVerticalLine(x, boldness_line, stipple_line);

    const int upper_unit_y{marginCorrected(Y_DISPLAY_RESOLUTION) +
                           static_cast<uint16_t>(2 * VERY_BOLD)};
    const int upper_value_y{upper_unit_y + 20};
    const int lower_value_y{marginCorrected(0) -
                            static_cast<uint16_t>(2 * VERY_BOLD) - 20};
    const int lower_unit_y{lower_value_y - 20};

    drawText(marginCorrected(x) - 20, upper_value_y, value_label, font_size, color_rgb, boldness_text);
    drawText(marginCorrected(x) - 10, upper_unit_y, unit_label, font_size, color_rgb, boldness_text);
    drawText(marginCorrected(x) - 20, lower_value_y, value_label, font_size, color_rgb, boldness_text);
    drawText(marginCorrected(x) - 10, lower_unit_y, unit_label, font_size, color_rgb, boldness_text);
}

static void drawTriggerIndicator(const int x, const int y)
{
    const std::string voltage_value{std::to_string(scaleYToVoltage_mV(y))};

    glColor3f(COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY);

    drawVerticalLine(x, NEUTRAL);
    drawHorizontalLineWithLabels(y, voltage_value.c_str(), "mV", FONT_SIZE_LARGE, COLOR_RGB_WHITE, NEUTRAL, NEUTRAL);
}

static void drawHorizontalGrid(const int numOfHorizontalLayers)
{
    const int middle_y{Y_DISPLAY_RESOLUTION / 2};
    const std::string voltage_str{std::to_string(scaleYToVoltage_mV(middle_y))};

    drawHorizontalLineWithLabels(middle_y, voltage_str.c_str(), " mV", FONT_SIZE_SMALL, COLOR_RGB_DARK_GRAY, NEUTRAL, BOLD, true);

    const float delta_y{Y_DISPLAY_RESOLUTION / (2 * numOfHorizontalLayers)};
    for (int i = 1; i <= numOfHorizontalLayers; ++i)
    {
        const int upper_y{middle_y + (i * delta_y)};
        const int lower_y{middle_y - (i * delta_y)};

        const std::string upper_voltage_str{std::to_string(scaleYToVoltage_mV(upper_y))};
        const std::string lower_voltage_str{std::to_string(scaleYToVoltage_mV(lower_y))};

        drawHorizontalLineWithLabels(upper_y, upper_voltage_str.c_str(), " mV", FONT_SIZE_SMALL, COLOR_RGB_DARK_GRAY, NEUTRAL, BOLD, true);
        drawHorizontalLineWithLabels(lower_y, lower_voltage_str.c_str(), " mV", FONT_SIZE_SMALL, COLOR_RGB_DARK_GRAY, NEUTRAL, BOLD, true);
    }
}

static void drawVerticalGrid(const int numOfVerticalLayers)
{
    const int middle_x{X_DISPLAY_RESOLUTION / 2};

    drawVerticalLineWithLabels(middle_x, "0", " us", FONT_SIZE_SMALL, COLOR_RGB_DARK_GRAY, NEUTRAL, BOLD, true);

    const float delta_x{X_DISPLAY_RESOLUTION / (2 * numOfVerticalLayers)};
    for (int i = 1; i <= numOfVerticalLayers; ++i)
    {
        const int right_x{middle_x + (i * delta_x)};
        const int left_x{middle_x - (i * delta_x)};

        drawVerticalLineWithLabels(right_x, nullptr, " us", FONT_SIZE_SMALL, COLOR_RGB_DARK_GRAY, NEUTRAL, BOLD, true);
        drawVerticalLineWithLabels(left_x, nullptr, " us", FONT_SIZE_SMALL, COLOR_RGB_DARK_GRAY, NEUTRAL, BOLD, true);
    }
}

static void drawGrid(const int numOfVerticalLayers,
                     const int numOfHorizontalLayers)
{
    glColor3f(COLOR_RGB_DARK_GRAY, COLOR_RGB_DARK_GRAY, COLOR_RGB_DARK_GRAY);

    drawHorizontalGrid(numOfHorizontalLayers);
    drawVerticalGrid(numOfVerticalLayers);
}

static void drawDisplayAreaBorder()
{
    glColor3f(COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY);

    constexpr float HALF_THICKNESS{VERY_BOLD / 2};

    drawVerticalLine(-1 * HALF_THICKNESS, VERY_BOLD);
    drawVerticalLine(X_DISPLAY_RESOLUTION + HALF_THICKNESS, VERY_BOLD);
    drawHorizontalLine(-1 * HALF_THICKNESS, VERY_BOLD);
    drawHorizontalLine(Y_DISPLAY_RESOLUTION + HALF_THICKNESS, VERY_BOLD);
}
