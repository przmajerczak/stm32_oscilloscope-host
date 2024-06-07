#pragma once

#include "constants.hpp"

static void drawTriggerIndicator(const int x, const int y)
{
    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    glVertex2f(x, 0);
    glVertex2f(x, Y_SIZE);
    glVertex2f(0, y);
    glVertex2f(X_SIZE, y);

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
        const int x{static_cast<int>(X_SIZE * (static_cast<double>(i) / (numOfVerticalLines + 1)))};
        glVertex2f(x, 0);
        glVertex2f(x, Y_SIZE);
    }

    for (int i = 1; i <= numOfHorizontalLines; ++i)
    {
        const int y{static_cast<int>(Y_SIZE * (static_cast<double>(i) / (numOfHorizontalLines + 1)))};
        glVertex2f(0, y);
        glVertex2f(X_SIZE, y);
    }

    glEnd();
    glDisable(GL_LINE_STIPPLE);
}
