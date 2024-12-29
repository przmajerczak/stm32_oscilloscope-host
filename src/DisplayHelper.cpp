#include "DisplayHelper.hpp"
#include "SettingsWindow.hpp"

#include "constants.hpp"
#include "utils.hpp"

namespace
{
    bool initial_display{true};
    bool display_trigger_flag{false};
    RawValuesContainer rawValues;
} // namespace

void DisplayHelper::triggerDisplay(const RawValuesContainer &values)
{
    rawValues = values;
    display_trigger_flag = true;
}

void DisplayHelper::display()
{
    if (initial_display)
    {
        drawGrid(10, 8);
        drawTriggerIndicator((X_SIZE / 2),
                             scaleRawValueToScreenHeight(
                                 SettingsWindow::getTriggerThresholdSliderValue()));

        glFlush();
        initial_display = false;
    }
    else if (display_trigger_flag)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        drawGrid(10, 8);

        glPointSize(1.0);

        glColor3f(1.0, 1.0, 0.0);
        glLineWidth(2.0);

        glBegin(GL_LINES);

        uint16_t x{0};

        auto value_it{rawValues.begin()};

        while (*value_it == INVALID_VALUE)
        {
            x += X_LENGTH;
            ++value_it;
        }

        uint16_t y{scaleRawValueToScreenHeight(*value_it)};
        ++value_it;

        glVertex2f(x, y);

        for (value_it;
             value_it != rawValues.end(); ++value_it)
        {

            if (*value_it == INVALID_VALUE)
            {
                break;
            }
            x += X_LENGTH;
            y = scaleRawValueToScreenHeight(*value_it);

            glVertex2f(x, y);
            glVertex2f(x, y);
        }

        glVertex2f(x, y);

        glEnd();

        drawTriggerIndicator((X_SIZE / 2),
                             scaleRawValueToScreenHeight(
                                 SettingsWindow::getTriggerThresholdSliderValue()));

        glFlush();

        display_trigger_flag = false;
    }

    glutPostRedisplay();
}

void DisplayHelper::init(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(X_SIZE, Y_SIZE);
    glutInitWindowPosition(X_POSITION, Y_POSITION);
    glutCreateWindow("STM32 Oscilloscope");

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, X_SIZE, 0.0, Y_SIZE);
}

void DisplayHelper::run()
{
    glutDisplayFunc(display);
    glutMainLoop();
}
