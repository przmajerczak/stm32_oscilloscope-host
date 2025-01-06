#pragma once

#include "constants.hpp"
#include "utils.hpp"
#include <GL/glut.h>

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

static void drawTriggerIndicator(const int x, const int y)
{
    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    glVertex2f(marginCorrected(x), marginCorrected(0));
    glVertex2f(marginCorrected(x), marginCorrected(Y_DISPLAY_RESOLUTION));
    glVertex2f(marginCorrected(0), marginCorrected(y));
    glVertex2f(marginCorrected(X_DISPLAY_RESOLUTION), marginCorrected(y));

    glEnd();
}

static void drawGrid(const int numOfVerticalLines, const int numOfHorizontalLines)
{
    glColor3f(0.3, 0.3, 0.3);

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = 1; i <= numOfVerticalLines; ++i)
    {
        const int x{static_cast<int>(X_DISPLAY_RESOLUTION * (static_cast<double>(i) / (numOfVerticalLines + 1)))};
        glVertex2f(marginCorrected(x), marginCorrected(0));
        glVertex2f(marginCorrected(x), marginCorrected(Y_DISPLAY_RESOLUTION));
    }

    for (int i = 1; i <= numOfHorizontalLines; ++i)
    {
        const int y{static_cast<int>(Y_DISPLAY_RESOLUTION * (static_cast<double>(i) / (numOfHorizontalLines + 1)))};
        glVertex2f(marginCorrected(0), marginCorrected(y));
        glVertex2f(marginCorrected(X_DISPLAY_RESOLUTION), marginCorrected(y));
    }

    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

static void drawDisplayAreaBorder()
{
    glColor3f(0.6, 0.6, 0.6);
    glLineWidth(BOLD_THICKNESS);

    glBegin(GL_LINE_LOOP);

    constexpr float HALF_THICKNESS{BOLD_THICKNESS / 2};
    glVertex2f(marginCorrected(0) - HALF_THICKNESS, marginCorrected(0) - HALF_THICKNESS);
    glVertex2f(marginCorrected(0) - HALF_THICKNESS, marginCorrected(Y_DISPLAY_RESOLUTION) + HALF_THICKNESS);
    glVertex2f(marginCorrected(X_DISPLAY_RESOLUTION) + HALF_THICKNESS, marginCorrected(Y_DISPLAY_RESOLUTION) + HALF_THICKNESS);
    glVertex2f(marginCorrected(X_DISPLAY_RESOLUTION) + HALF_THICKNESS, marginCorrected(0) - HALF_THICKNESS);

    glEnd();
}
