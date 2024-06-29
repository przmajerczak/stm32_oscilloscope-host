#include "SettingsWindow.hpp"

#include <gtk/gtk.h>
#include <thread>
#include "constants.hpp"

namespace
{
    GtkWidget *window = nullptr;
    GtkWidget *grid = nullptr;
}

void SettingsWindow::init()
{
    gtk_init(nullptr, nullptr);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Settings");
    gtk_window_move(GTK_WINDOW(window), X_POSITION + X_SIZE, Y_POSITION);
    gtk_window_set_default_size(GTK_WINDOW(window), X_SIZE / 4, Y_SIZE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
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
