#include "TemporaryFrequencyControls.hpp"

#include "sharedData/DynamicData.hpp"

#include "constants.hpp"
#include <iomanip>
#include <sstream>

gboolean frequencyLabelTimeoutAction(gpointer _callbackData)
{
    CallbackData *callbackData = (CallbackData *)_callbackData;
    GtkWidget *frequencyLabel = callbackData->widget;
    DynamicData *dynamicData = callbackData->dynamicData;

    uint16_t timeoutsWithoutReset = 0;

    GtkLabel *label = GTK_LABEL(frequencyLabel);

    constexpr uint32_t minReliableLimit{5};
    auto &thresholdTriggersSinceLastFreqLabelReset{
        dynamicData->thresholdTriggersSinceLastFreqLabelReset};

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

void TemporaryFrequencyControls::prepare(DynamicData &dynamicData)
{
    frequencyLabel = gtk_label_new("Signal frequency: --- Hz");
    gtk_widget_set_hexpand(frequencyLabel, TRUE);

    callbackData.widget = frequencyLabel;
    callbackData.dynamicData = &dynamicData;

    g_timeout_add(FREQUENCY_LABEL_TIMEOUT_MS, frequencyLabelTimeoutAction,
                  &callbackData);
}

GtkWidget *TemporaryFrequencyControls::getFrequencyControlsContainer()
{
    constexpr int spacing{0};
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    constexpr int padding{0};
    gtk_box_pack_start(GTK_BOX(vbox), frequencyLabel, FALSE, TRUE, padding);

    return vbox;
}
