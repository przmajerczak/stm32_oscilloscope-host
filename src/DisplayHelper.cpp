#include "DisplayHelper.hpp"
#include "SettingsWindow.hpp"

#include "constants.hpp"
#include <GL/glut.h>
#include "utils.hpp"

namespace
{
    bool display_trigger_flag{true};
    RawValuesContainer rawValues{0};
} // namespace

void DisplayHelper::triggerDisplay(const RawValuesContainer &values)
{
    rawValues = std::move(values);
    display_trigger_flag = true;
}

void DisplayHelper::display()
{
    if (display_trigger_flag)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        drawGrid(10, 8);

        glPointSize(1.0);

        glColor3f(1.0, 1.0, 0.0);
        glLineWidth(2.0);

        glBegin(GL_LINES);

        int x{marginCorrected(0)};

        auto value_it{rawValues.begin()};

        while (*value_it == INVALID_VALUE)
        {
            x += X_LENGTH;
            ++value_it;
        }

        uint16_t y{marginCorrected(scaleRawValueToDisplayHeight(*value_it))};
        ++value_it;

        glVertex2f(x, y);

        for (value_it; value_it != rawValues.end(); ++value_it)
        {

            if (*value_it == INVALID_VALUE)
            {
                break;
            }
            x += X_LENGTH;
            y = marginCorrected(scaleRawValueToDisplayHeight(*value_it));

            glVertex2f(x, y);
            glVertex2f(x, y);
        }

        glVertex2f(x, y);

        glEnd();

        drawTriggerIndicator(
            (X_DISPLAY_RESOLUTION / 2),
            scaleRawValueToDisplayHeight(
                SettingsWindow::getTriggerThresholdSliderValue()));
        drawDisplayAreaBorder();

        glFlush();

        display_trigger_flag = false;
    }

    glutPostRedisplay();
}

void DisplayHelper::init(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(X_WINDOW_SIZE, Y_WINDOW_SIZE);
    glutInitWindowPosition(X_INITIAL_WINDOW_POSITION, Y_INITIAL_WINDOW_POSITION);
    glutCreateWindow("STM32 Oscilloscope");

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, X_WINDOW_SIZE, 0.0, Y_WINDOW_SIZE);
}

void DisplayHelper::run()
{
    glutDisplayFunc(display);
    glutMainLoop();
}
