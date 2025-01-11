#include "SettingsWindow.hpp"

#include "constants.hpp"
#include <thread>

#include "controls/VerticalBoundControls.hpp"
#include "controls/TriggerControls.hpp"
#include "controls/TemporaryFrequencyControls.hpp"

namespace
{
    GtkWidget *window = nullptr;
} // namespace

void SettingsWindow::configure()
{
    gtk_init(nullptr, nullptr);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Settings");
    gtk_window_move(GTK_WINDOW(window), X_INITIAL_WINDOW_POSITION + X_WINDOW_SIZE,
                    Y_INITIAL_WINDOW_POSITION);
    gtk_window_set_default_size(GTK_WINDOW(window), X_WINDOW_SIZE / 3,
                                Y_WINDOW_SIZE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);
}

void SettingsWindow::fill()
{
    TemporaryFrequencyControls::prepare();
    TriggerControls::prepare();
    VerticalBoundControls::prepare();

    GtkWidget *verticalBoundControlsGrid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(verticalBoundControlsGrid), VerticalBoundControls::verticalLowerBoundSlider(), 0, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(verticalBoundControlsGrid), VerticalBoundControls::verticalUpperBoundSlider(), 0, 6, 2, 1);

    constexpr int spacing{0};
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    constexpr int padding{0};
    gtk_box_pack_start(GTK_BOX(vbox), TemporaryFrequencyControls::getFrequencyControlsContainer(), FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(vbox), TriggerControls::getTriggerControlsContainer(), FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(vbox), verticalBoundControlsGrid, FALSE, TRUE, padding);
}

void SettingsWindow::runAsSeparateThread()
{
    std::thread gtk_thread([&]()
    {
        configure();
        fill();

        gtk_widget_show_all(window);
        gtk_main();
    });

    gtk_thread.detach();
}
