#pragma once

#include "constants.hpp"
#include "utils.hpp"
#include <GL/glut.h>
#include <string>

static void drawText(const float x, const float y, const char *text,
                     const float scale, const float boldness = 1.0,
                     const float color_r = 1.0f, const float color_g = 1.0f,
                     const float color_b = 1.0f)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(color_r, color_g, color_b);
    glLineWidth(boldness);
    glScalef(scale, scale, scale);
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

static void drawTriggerIndicator(const int x, const int y)
{
    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(1.0);

    drawHorizontalLine(y);
    drawVerticalLine(x);

    const int voltage_mV{static_cast<int>(MAX_VOLTAGE_mV * static_cast<float>(y) /
                                          static_cast<float>(Y_DISPLAY_RESOLUTION))};
    const std::string voltage_value{std::to_string(voltage_mV)};
    const std::string voltage_unit{"mV"};

    const int right_x{marginCorrected(X_DISPLAY_RESOLUTION) +
                      static_cast<uint16_t>(2 * BOLD_THICKNESS)};
    const int left_x{static_cast<uint16_t>(2 * BOLD_THICKNESS)};
    const int upper_line_y{marginCorrected(y)};
    const int lower_line_y{upper_line_y - 20}; // TODO: remove magic number

    constexpr float FONT_SIZE{0.15f};

    drawText(right_x, upper_line_y, voltage_value.c_str(), FONT_SIZE);
    drawText(right_x, lower_line_y, voltage_unit.c_str(), FONT_SIZE);
    drawText(left_x, upper_line_y, voltage_value.c_str(), FONT_SIZE);
    drawText(left_x, lower_line_y, voltage_unit.c_str(), FONT_SIZE);
}

static void drawGrid(const int numOfVerticalLines, const int numOfHorizontalLines)
{
    glColor3f(0.3, 0.3, 0.3);

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
    glLineWidth(1.0);

    for (int i = 1; i <= numOfVerticalLines; ++i)
    {
        const int x{static_cast<int>(X_DISPLAY_RESOLUTION * (static_cast<double>(i) / (numOfVerticalLines + 1)))};
        drawVerticalLine(x);
    }

    for (int i = 1; i <= numOfHorizontalLines; ++i)
    {
        const int y{static_cast<int>(Y_DISPLAY_RESOLUTION * (static_cast<double>(i) / (numOfHorizontalLines + 1)))};
        drawHorizontalLine(y);
    }

    glDisable(GL_LINE_STIPPLE);
}

static void drawDisplayAreaBorder()
{
    glColor3f(0.6, 0.6, 0.6);
    glLineWidth(BOLD_THICKNESS);

    constexpr float HALF_THICKNESS{BOLD_THICKNESS / 2};

    drawVerticalLine(-1 * HALF_THICKNESS);
    drawVerticalLine(X_DISPLAY_RESOLUTION + HALF_THICKNESS);
    drawHorizontalLine(-1 * HALF_THICKNESS);
    drawHorizontalLine(Y_DISPLAY_RESOLUTION + HALF_THICKNESS);
}
