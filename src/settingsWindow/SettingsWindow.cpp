#include "SettingsWindow.hpp"

#include "constants.hpp"
#include "utils.hpp"
#include <iomanip>
#include <sstream>
#include <thread>

#include "VerticalBoundControls.hpp"
#include "TriggerControls.hpp"

namespace
{
    GtkWidget *window = nullptr;
    GtkWidget *grid = nullptr;
    GtkWidget *frequencyLabel = nullptr;

    uint32_t thresholdTriggersSinceLastFreqLabelReset = 0;
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

    frequencyLabel = gtk_label_new("Signal frequency: --- Hz");
    gtk_widget_set_hexpand(frequencyLabel, TRUE);
    gtk_grid_attach(GTK_GRID(grid), frequencyLabel, 0, 0, 2, 1);
    g_timeout_add(FREQUENCY_LABEL_TIMEOUT_MS, frequencyLabelTimeoutAction,
                  frequencyLabel);
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

void SettingsWindow::notifyAboutThresholdTrigger()
{
    ++thresholdTriggersSinceLastFreqLabelReset;
}

gboolean SettingsWindow::frequencyLabelTimeoutAction(gpointer widgetPtr)
{
    static uint16_t timeoutsWithoutReset = 0;

    GtkLabel *label = GTK_LABEL(widgetPtr);

    constexpr uint32_t minReliableLimit{5};

    if (thresholdTriggersSinceLastFreqLabelReset > minReliableLimit)
    {
        const double frequency_Hz =
            1000.0 * static_cast<double>(thresholdTriggersSinceLastFreqLabelReset) /
            static_cast<double>(FREQUENCY_LABEL_TIMEOUT_MS *
                                (1 + timeoutsWithoutReset));

        std::stringstream labelContent;
        labelContent << "Signal frequency: " << std::fixed << std::setprecision(2)
                     << frequency_Hz << " Hz";

        gtk_label_set_text(label, labelContent.str().c_str());

        thresholdTriggersSinceLastFreqLabelReset = 0;
        timeoutsWithoutReset = 0;
    }
    else
    {
        ++timeoutsWithoutReset;
    }

    return TRUE;
}

// TODO: remove.
// totally unused, but trigger slider stops working when removed
uint16_t SettingsWindow::getTriggerThresholdY()
{
    return TriggerControls::getTriggerThresholdY();
}

ThresholdTrigger SettingsWindow::getThresholdTrigger()
{
    return TriggerControls::getThresholdTrigger();
}
