#include "SettingsWindow.hpp"

#include "constants.hpp"
#include "utils.hpp"
#include <thread>

#include "controls/VerticalBoundControls.hpp"
#include "controls/TriggerControls.hpp"
#include "controls/TemporaryFrequencyControls.hpp"

namespace
{
    GtkWidget *window = nullptr;
    GtkWidget *grid = nullptr;
} // namespace

void SettingsWindow::init()
{
    gtk_init(nullptr, nullptr);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Settings");
    gtk_window_move(GTK_WINDOW(window), X_INITIAL_WINDOW_POSITION + X_WINDOW_SIZE,
                    Y_INITIAL_WINDOW_POSITION);
    gtk_window_set_default_size(GTK_WINDOW(window), X_WINDOW_SIZE / 3,
                                Y_WINDOW_SIZE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    TemporaryFrequencyControls::prepare();

    gtk_grid_attach(GTK_GRID(grid), TemporaryFrequencyControls::getFrequencyLabel(), 0, 0, 2, 1);

    TriggerControls::prepare();

    gtk_grid_attach(GTK_GRID(grid), TriggerControls::getThresholdLabel(), 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), TriggerControls::getTriggerThresholdSlider(), 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), TriggerControls::getTriggerLabel(), 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), TriggerControls::getTriggerRisingEdgeButton(), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), TriggerControls::getTriggerFallingEdgeButton(), 1, 4, 1, 1);

    VerticalBoundControls::prepare();

    gtk_grid_attach(GTK_GRID(grid), VerticalBoundControls::verticalLowerBoundSlider(), 0, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), VerticalBoundControls::verticalUpperBoundSlider(), 0, 6, 2, 1);
}

void SettingsWindow::run()
{
    std::thread gtk_thread([&]()
    {
        init();
        gtk_widget_show_all(window);
        gtk_main();
    });

    gtk_thread.detach();
}
