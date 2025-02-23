#include "SettingsWindow.hpp"

#include "sharedData/constants.hpp"
#include <thread>

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

void SettingsWindow::fill(DynamicData &dynamicData)
{
    triggerControls.prepare(dynamicData);
    verticalBoundControls.prepare(dynamicData);
    measurementsControls.prepare(dynamicData);

    constexpr int spacing{0};
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    constexpr int padding{0};
    gtk_box_pack_start(GTK_BOX(vbox), triggerControls.getTriggerControlsContainer(), FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(vbox), verticalBoundControls.getVerticalBoundControlsContainer(), FALSE, TRUE, padding);
    gtk_box_pack_start(GTK_BOX(vbox), measurementsControls.getFrequencyControlsContainer(), FALSE, TRUE, padding);
}

void SettingsWindow::runAsSeparateThread(DynamicData &dynamicData)
{
    std::thread gtk_thread([&]()
    {
        configure();
        fill(dynamicData);

        gtk_widget_show_all(window);
        gtk_main();
    });

    gtk_thread.detach();
}
