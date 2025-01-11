#include "TemporaryFrequencyControls.hpp"

#include "sharedData/DynamicData.hpp"

#include "constants.hpp"
#include <iomanip>
#include <sstream>

gboolean frequencyLabelTimeoutAction(gpointer widgetPtr)
{
    uint16_t timeoutsWithoutReset = 0;

    GtkLabel *label = GTK_LABEL(widgetPtr);

    constexpr uint32_t minReliableLimit{5};
    auto &thresholdTriggersSinceLastFreqLabelReset{
        DynamicData::getThresholdTriggersSinceLastFreqLabelReset()};

    if (thresholdTriggersSinceLastFreqLabelReset > minReliableLimit)
    {
        const double frequency_Hz =
            1000.0 * static_cast<double>(thresholdTriggersSinceLastFreqLabelReset) /
            static_cast<double>(FREQUENCY_LABEL_TIMEOUT_MS * (1 + timeoutsWithoutReset));

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

void TemporaryFrequencyControls::prepare()
{
    frequencyLabel = gtk_label_new("Signal frequency: --- Hz");
    gtk_widget_set_hexpand(frequencyLabel, TRUE);
    g_timeout_add(FREQUENCY_LABEL_TIMEOUT_MS, frequencyLabelTimeoutAction,
                  frequencyLabel);
}

GtkWidget *TemporaryFrequencyControls::getFrequencyControlsContainer()
{
    GtkWidget *frequencyControlsGrid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(frequencyControlsGrid), frequencyLabel, 0, 0, 2, 1);

    return frequencyControlsGrid;
}
