#pragma once

#include "constants.hpp"
#include "utils.hpp"
#include <GL/glut.h>
#include <string>

static void drawText(const float x, const float y, const char *text,
                     const float font_size, const float color_rgb,
                     const float boldness = 1.0)
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

static void drawHorizontalLine(const int y)
{
    glBegin(GL_LINES);

    glVertex2f(marginCorrected(0), marginCorrected(y));
    glVertex2f(marginCorrected(X_DISPLAY_RESOLUTION), marginCorrected(y));

    glEnd();
}

static void drawVerticalLine(const int x)
{
    glBegin(GL_LINES);

    glVertex2f(marginCorrected(x), marginCorrected(0));
    glVertex2f(marginCorrected(x), marginCorrected(Y_DISPLAY_RESOLUTION));

    glEnd();
}

static void drawHorizontalLineWithLabels(const int y, const char *value_label, const char *unit_label,
                                         const float font_size, const float color_rgb)
{
    drawHorizontalLine(y);

    const int right_x{marginCorrected(X_DISPLAY_RESOLUTION) +
                      static_cast<uint16_t>(2 * BOLD_THICKNESS)};
    const int left_x{static_cast<uint16_t>(2 * BOLD_THICKNESS)};
    const int upper_line_y{marginCorrected(y)};
    const int lower_line_y{upper_line_y - 20}; // TODO: remove magic number

    drawText(right_x, upper_line_y, value_label, font_size, color_rgb);
    drawText(right_x, lower_line_y, unit_label, font_size, color_rgb);
    drawText(left_x, upper_line_y, value_label, font_size, color_rgb);
    drawText(left_x, lower_line_y, unit_label, font_size, color_rgb);
}

static void drawVerticalLineWithLabels(const int x, const char *value_label, const char *unit_label,
                                       const float font_size, const float color_rgb)
{
    drawVerticalLine(x);

    const int upper_unit_y{marginCorrected(Y_DISPLAY_RESOLUTION) +
                           static_cast<uint16_t>(2 * BOLD_THICKNESS)};
    const int upper_value_y{upper_unit_y + 20};
    const int lower_value_y{marginCorrected(0) -
                            static_cast<uint16_t>(2 * BOLD_THICKNESS) - 20};
    const int lower_unit_y{lower_value_y - 20};

    drawText(marginCorrected(x) - 20, upper_value_y, value_label, font_size, color_rgb);
    drawText(marginCorrected(x) - 10, upper_unit_y, unit_label, font_size, color_rgb);
    drawText(marginCorrected(x) - 20, lower_value_y, value_label, font_size, color_rgb);
    drawText(marginCorrected(x) - 10, lower_unit_y, unit_label, font_size, color_rgb);
}

static void drawTriggerIndicator(const int x, const int y)
{
    const int voltage_mV{
        static_cast<int>(MAX_VOLTAGE_mV * static_cast<float>(y) /
                         static_cast<float>(Y_DISPLAY_RESOLUTION))};
    const std::string voltage_value{std::to_string(voltage_mV)};

    glColor3f(COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY);
    glLineWidth(1.0);

    drawVerticalLine(x);
    drawHorizontalLineWithLabels(y, voltage_value.c_str(), "mV", FONT_SIZE_LARGE, COLOR_RGB_WHITE);
}

static void drawGrid(const int numOfVerticalLines,
                     const int numOfHorizontalLines)
{
    glColor3f(COLOR_RGB_DARK_GRAY, COLOR_RGB_DARK_GRAY, COLOR_RGB_DARK_GRAY);

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
    glLineWidth(1.0);

    for (int i = 1; i <= numOfVerticalLines; ++i)
    {
        const int x{
            static_cast<int>(X_DISPLAY_RESOLUTION *
                             (static_cast<double>(i) / (numOfVerticalLines + 1)))};
        drawVerticalLine(x);
    }

    for (int i = 1; i <= numOfHorizontalLines; ++i)
    {
        const int y{
            static_cast<int>(Y_DISPLAY_RESOLUTION * (static_cast<double>(i) /
                                                     (numOfHorizontalLines + 1)))};
        drawHorizontalLine(y);
    }

    glDisable(GL_LINE_STIPPLE);
}

static void drawDisplayAreaBorder()
{
    glColor3f(COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY, COLOR_RGB_LIGHT_GRAY);
    glLineWidth(BOLD_THICKNESS);

    constexpr float HALF_THICKNESS{BOLD_THICKNESS / 2};

    drawVerticalLine(-1 * HALF_THICKNESS);
    drawVerticalLine(X_DISPLAY_RESOLUTION + HALF_THICKNESS);
    drawHorizontalLine(-1 * HALF_THICKNESS);
    drawHorizontalLine(Y_DISPLAY_RESOLUTION + HALF_THICKNESS);
}
