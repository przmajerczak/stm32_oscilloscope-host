#include "TemporaryFrequencyControls.hpp"

#include "constants.hpp"
#include <iomanip>
#include <sstream>

namespace
{
    GtkWidget *frequencyLabel = nullptr;
    uint32_t thresholdTriggersSinceLastFreqLabelReset = 0;
} // namespace

void TemporaryFrequencyControls::prepare()
{
    frequencyLabel = gtk_label_new("Signal frequency: --- Hz");
    gtk_widget_set_hexpand(frequencyLabel, TRUE);
    g_timeout_add(FREQUENCY_LABEL_TIMEOUT_MS, frequencyLabelTimeoutAction,
                  frequencyLabel);
}

void TemporaryFrequencyControls::notifyAboutThresholdTrigger()
{
    ++getThresholdTriggersSinceLastFreqLabelReset();
}

GtkWidget *TemporaryFrequencyControls::getFrequencyLabel() { return frequencyLabel; }

gboolean TemporaryFrequencyControls::frequencyLabelTimeoutAction(gpointer widgetPtr)
{
    uint16_t timeoutsWithoutReset = 0;

    GtkLabel *label = GTK_LABEL(widgetPtr);

    constexpr uint32_t minReliableLimit{5};

    if (getThresholdTriggersSinceLastFreqLabelReset() > minReliableLimit)
    {
        const double frequency_Hz =
            1000.0 * static_cast<double>(getThresholdTriggersSinceLastFreqLabelReset()) /
            static_cast<double>(FREQUENCY_LABEL_TIMEOUT_MS *
                                (1 + timeoutsWithoutReset));

        std::stringstream labelContent;
        labelContent << "Signal frequency: " << std::fixed << std::setprecision(2)
                     << frequency_Hz << " Hz";

        gtk_label_set_text(label, labelContent.str().c_str());

        getThresholdTriggersSinceLastFreqLabelReset() = 0;
        timeoutsWithoutReset = 0;
    }
    else
    {
        ++timeoutsWithoutReset;
    }

    return TRUE;
}

// TODO: get rid of it when class became non-static
uint32_t &TemporaryFrequencyControls::getThresholdTriggersSinceLastFreqLabelReset()
{
    return thresholdTriggersSinceLastFreqLabelReset;
}