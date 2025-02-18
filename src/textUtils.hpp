#pragma once

#include "constants.hpp"
#include <GL/glut.h>

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
