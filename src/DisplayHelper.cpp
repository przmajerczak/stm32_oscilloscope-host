#include "DisplayHelper.hpp"

#include "constants.hpp"
#include "utils.hpp"

namespace
{
    bool initial_display{true};
    bool display_trigger_flag{false};
    RawValuesContainer raw_retrieved_values;
}

void DisplayHelper::triggerDisplay(const RawValuesContainer values)
{
    raw_retrieved_values = values;
    display_trigger_flag = true;
}

void DisplayHelper::display()
{
    if (initial_display)
    {
        drawGrid(10, 8);
        glFlush();
        initial_display = false;
    }
    else if (display_trigger_flag)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        const int x_length{(X_SIZE / static_cast<int>(raw_retrieved_values.size())) / 2};

        int x{X_SIZE / 2};
        int y{static_cast<int>(raw_retrieved_values.front())};
        y = scaleRawValueToScreenHeight(y);

        drawGrid(10, 8);
        drawTriggerIndicator(x, y);

        glPointSize(1.0);

        glColor3f(1.0, 1.0, 0.0);
        glLineWidth(2.0);

        glBegin(GL_LINES);
        glVertex2f(x, y);

        for (auto value_it = std::next(raw_retrieved_values.begin(), 1); value_it != raw_retrieved_values.end(); ++value_it)
        {

            x += x_length;
            y = scaleRawValueToScreenHeight(*value_it);

            glVertex2f(x, y);
            glVertex2f(x, y);
        }
        glVertex2f(x, y);

        glEnd();
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
    glutInitWindowPosition(100, 100);
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
